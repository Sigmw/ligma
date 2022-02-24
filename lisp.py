#!/usr/bin/env python3
import math
import operator
import sys
import traceback
import types

def readfile(a):
    with open(a, 'r') as f:
        return f.read()

def atom(i):
    try:
        return int(i)
    except:
        try:
            return float(i)
        except:
            return i.strip()


def parseAtom(val, cdr, car):
    val = atom(val)
    if val is not None and val != '':
        if cdr is None:
            cdr = val
        elif car is None:
            car = val
        else:
            car = (car, val)
    val = ''

    return (val, cdr, car)

def parsePair(prg, level=0):
    res = []
    partials = ''
    idx = 0
    dest = len(prg)
    cdr = None
    car = None
    while idx < dest:
        c = prg[idx]
        #print ( '+cc %s %s %s' % (c,res, idx))
        if c == '"' or (partials and partials[0] == '"'):
            if partials:
                if partials[0] == '"':
                    partials += c
                if c == '"':
                    # FIXME
                    partials, cdr, car = parseAtom(partials, cdr, car)
                    partials = ''
            else:
                partials = '"'
            idx += 1
        elif c == ' ':
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('GGP %s, %s' % (cdr, car))
            #partials = ''
            idx += 1
        elif c == '(':
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('PPP %s, %s' % (cdr, car))
            rest = prg[idx + 1:]
            #a, b = parsePair(rest)
            #idx += b + 1
            a, b, c = parsePair(rest, level + 1)
            idx += c + 1
            #print ('CCC' , a,b)
            if cdr is None:
                cdr = a
                car = b
            elif car is None:
                car = (a, b)
            else:
                #cdr = (cdr, car)
                #car = (a, b)
                car = (car, (a, b))
            if level == 0:
                return (cdr, car, idx)
            #print ('CCP %s, %s' % (cdr, car))
        elif c == ')':
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('RRP %s, %s' % (cdr, car))
            return (cdr, car, idx + 1)
            #return res, idx + 1
        else:
            partials += c
            idx += 1

    partials, cdr, car = parseAtom(partials, cdr, car)
    return (cdr, car, idx)
    #return res, idx

def parse(prg):
    """
    >>> parse("")
    (None, None)
    >>> parse("()")
    (None, None)
    >>> parse("(1)")
    (1, None)
    >>> parse("(1 2)")
    (1, 2)
    >>> parse("(1, 2)")
    ('1,', 2)
    >>> parse("(+ 1 2)")
    ('+', (1, 2))
    >>> parse("(+ 1 2 3)")
    ('+', ((1, 2), 3))
    >>> parse("(* 2 (+ 1 3))")
    ('*', (2, ('+', (1, 3))))
    >>> parse("(* 2 (+ (* 2 1.2) 3))")
    ('*', (2, ('+', (('*', (2, 1.2)), 3))))
    >>> parse("(+ 1 2) (+ 2 3)")
    [('+', (1, 2)), ('+', (2, 3))]
    """
    res = []
    dest = len(prg)
    idx = 0
    l = 0
    while idx < dest and prg[idx:].strip():
        cdr, car, cc = parsePair(prg[idx:])
        res.append((cdr, car))
        idx += cc
    if len(res) > 1:
        return res
    elif res:
        return res[0]
    return (None, None)

class LambdaCall(object):
    def __init__(self, body, params, env):
        self.params = params
        self.body = body
        self.env = env

    def __call__(self, *args):
        #return stackEval(self.body, Env(env, self.params, args))
        return stackEval(self.body, Env(self.env, self.params, args))

class Env(dict):
    def __init__(self, parent=None, args=[], vals=()):
        #print (' EEE %s pp %s' % (args, vals))
        self.update(zip(args, vals))
        self.parent = parent

    def find(self, var):
        if var in self:
            return self[var]
        elif self.parent:
            return self.parent.find(var)

        return None

def parseArgs(v, env):
    res = []
    e = stackEval(v, env)
    res.append(e)
    return res

def mapOper(oper, var, env):
    opermap = {
        '+': lambda a, b: a+b,
        '-': lambda a, b: a-b,
        '*': lambda a, b: a*b,
        '/': lambda a, b: a/b
    }
    if type(var) == tuple:
        l = stackEval(var[0], env)
        r = stackEval(var[1], env)
    else:
        l = stackEval(var, env)
        r = 0
    while type(l) == tuple:
        l = opermap[oper](l[0], l[1])
    while type(r) == tuple:
        r = opermap[oper](r[0], r[1])
    #print ('POM', oper, l, r)
    res = opermap[oper](l, r)
    #print ('OM', oper, res)
    return res
    """
    print (var)
    if type(var) == tuple:
        r = stackEval(var[0], env)
        l = stackEval(var[1], env)
        #print (r,l)
        return (r, l)
    else:
        print (var)
        return stackEval(var, env)

    #print (r, l)
    """

def flatTuple(var, env):
    """
    >>> flatTuple((1, 2), Env())
    [1, 2]
    >>> flatTuple(((1, 2), 3), Env())
    [1, 2, 3]
    >>> flatTuple((1, (2, 3)), Env())
    [1, 2, 3]
    >>> flatTuple((), Env())
    []
    >>> flatTuple((1, (2, 3), (4, (5,6))), Env())
    [1, 2, 3, 4, 5, 6]
    >>> flatTuple((0, 0), Env())
    [0, 0]
    """
    data = []
    if type(var) == tuple:
        for i in var:
            l = stackEval(i, env)
            data += flatTuple(l, env)
    elif var is not None:
        data.append(var)

    return data

def mapFlatApply(oper, var, env):
    """
    >>> mapFlatApply('+', (1, 2), Env())
    3
    >>> mapFlatApply('*', (1, 2), Env())
    2
    >>> mapFlatApply('+', ((1, 2), 3), Env())
    6
    >>> mapFlatApply('*', ((1, 2), 3), Env())
    6
    >>> mapFlatApply('+', (1, (2, 3)), Env())
    6
    >>> mapFlatApply('+', (1, (2, 3), (4, (5,6))), Env())
    21
    >>> mapFlatApply('*', (1, (2, 3), (4, (5,6))), Env())
    720
    >>> mapFlatApply('*', (('+', (1, 1)), (2, 3)), Env())
    12
    """
    data = flatTuple(var, env)
    opermap = {
        '+': lambda a, b: a+b,
        '-': lambda a, b: a-b,
        '*': lambda a, b: a*b,
        '/': lambda a, b: a/b
    }

    op = opermap[oper]
    if not data:
        raise ValueError('Not enough data provided for \'%s\' %s' % (oper, var))
    ret = data[0]
    rest = data[1:]
    while rest:
        ret = op(ret, rest[0])
        rest = rest[1:]
    return ret

def stackEval(val, env):
    if val is None:
        return None
    if type(val) == tuple:
        if len(val) == 0:
            return None
        elif len(val) == 1:
            cdr = val[0]
            car = None
        else:
            cdr, car = val
        if type(cdr) == tuple:
            #print ('CC' , cdr)
            l = stackEval(cdr, env)
            r = stackEval(car, env)
            #return r
            if l is not None and r is not None:
                return (l, r)
                #return r
                raise ValueError('Unexpected: %s %s' % (l, r))
            elif r is not None:
                return r
        elif cdr == 'begin':
            return stackEval(car, env)
        elif cdr == 'define':
            if type(car) == tuple:
                var = car[0]
                if type(var) == tuple:
                    # FIXME: Broken big time
                    name = var[0][0]
                    args = var[0][1]
                    body = (var[1:], car[1])
                    env[name] = LambdaCall(body, args, env)
                else:
                    val = stackEval(car[1], env)
                    env[var] = val
                #return val
            else:
                raise ValueError('Unexpected define: %s' % val)
        elif cdr in ['+', '-', '*', '/']:
            #return stackEval(car[0], env) * stackEval(car[1], env)
            return mapFlatApply(cdr, car, env)
        elif cdr == 'or':
            return stackEval(car[0], env) or stackEval(car[1], env)
        elif cdr == 'equal?' or cdr == '=':
            return stackEval(car[0], env) == stackEval(car[1], env)
        elif cdr == 'cos':
            return math.cos(stackEval(car, env))
        elif cdr == 'if':
            test = car[0][0]
            then = car[0][1]
            elsed = car[1]
            res = stackEval(test, env)
            if res:
                e = stackEval(then, env)
            else:
                e = stackEval(elsed, env)
            return e
        elif env.find(cdr):
            item = env.find(cdr)
            #print ('CALL %s' %  cdr)
            #args = []
            args = parseArgs(car, env)
            #print ('CALL %s (%s)' %  (cdr, args))
            #stackEval(car, env)
            #if type(args) != tuple:
            #    args = [args]
            if type(item) == types.FunctionType or type(item) == types.LambdaType or isinstance(item, LambdaCall):
                return item(*args)
            else:
                print (args, car)
                return item
        else:
            #print ('APE', cdr)
            #print ('APE', car)
            l = stackEval(cdr, env)
            r = stackEval(car, env)
            #return val
            #raise ValueError('Unexpected tuple: %s' % str(val))
            return (l, r)
        #print (stackEval(car, env))
    elif type(val) == int or type(val) == float:
        return val
    elif type(val) == str:
        if val[0] == '"' or val[0] == '\'':
            return val
        v = env.find(val)
        if v is None:
            raise ValueError('Undefined variable: %s' % val)
        return v
    elif type(val) == list:
        return [stackEval(x, env) for x in val]
    else:
        raise ValueError('Unexpected entry: %s' % val)

def initEnv():
    env = Env()
    env.update(vars(math))
    return env

def tidyRes(res):
    """
    >>> tidyRes('')
    ''
    >>> tidyRes(())
    >>> tidyRes((None, None))
    >>> tidyRes(None)
    >>> tidyRes((1, 2))
    (1, 2)
    >>> tidyRes(1)
    1
    >>> tidyRes((1,))
    1
    >>> tidyRes((1, None))
    1
    >>> tidyRes((1, None, 2))
    (1, 2)
    >>> tidyRes((None, 1))
    1
    >>> tidyRes([1, 2])
    [1, 2]
    >>> tidyRes([None, 2])
    2
    >>> tidyRes([None, 1, 2])
    [1, 2]
    >>> tidyRes([None, 1, None, (3, None, 5), 2])
    [1, (3, 5), 2]
    """
    if res is None:
        return None
    if type(res) == tuple or type(res) == list:
        if len(res) == 0:
            return None
        elif res[0] is None:
            res = res[1:]
            res = tidyRes(res)
        elif len(res) == 1:
            return res[0]
        else:
            if type(res) == tuple:
                res = tuple([tidyRes(x) for x in res if x is not None])
            else:
                res = [tidyRes(x) for x in res if x is not None]
            if len(res) == 0:
                return None
            elif len(res) == 1:
                return res[0]
    return res

def repl():
    env = initEnv()
    while True:
        sys.stdout.write('> ')
        sys.stdout.flush()
        data = sys.stdin.readline()
        if data.strip() == 'exit':
            break
        try:
            prg = parse(data)
            res = stackEval(prg, env)
            res = tidyRes(res)
            if res is not None:
                print (res)
        except Exception as e:
            print ('ERROR: %s' % e)

def runFile(name):
    data = readfile(name)

    prg = parse(data)
    #print (prg)

    env = initEnv()

    print (tidyRes(stackEval(prg, env)))

def runFiles(files):
    for fname in files:
        try:
            runFile(fname)
        except Exception as e:
            print ('ERROR: %s' % e)
            traceback.print_exc()

if __name__ == '__main__':
    if len(sys.argv) > 1:
        runFiles(sys.argv[1:])
    else:
        repl()
