use std::iter;
use std::collections::LinkedList;

pub enum Expr {
    Symbol(String),
    Int(i64),
    Float(f64),
    Str(String),
    List(Vec<Expr>),
    Cons(Box<Expr>, Box<Expr>) // head, tail
}

/*
trait ParseData {
    fn walk_tree(&self, level : usize);
    fn level_str(&self, level : usize) -> String {
        let res : String = iter::repeat(" ").take(level).collect();
        return res;
    }
}

impl ParseData for usize {
    fn walk_tree(&self, level : usize) {
        println!("{}{}", self.level_str(level), self);
    }
}

impl ParseData for f32 {
    fn walk_tree(&self, level : usize) {
        println!("{}{}", self.level_str(level), self);
    }
}

impl ParseData for String {
    fn walk_tree(&self, level : usize) {
        println!("{}{}", self.level_str(level), self);
    }
    / *
    fn atom(&self) -> ParseData {
        match self.parse::<usize>() {
            Ok(n) => n,
            Err(err) => match self.parse::<f32>() {
                Ok(n) => n,
                Err(err) => self
            },
        }
    }
    * /
}

impl ParseData for LinkedList<Box<ParseData>> {
    fn walk_tree(&self, level : usize) {
        println!("{}--", self.level_str(level));
        for val in self {
            val.walk_tree(level + 1);
        }
    }
}
*/

fn parse_number(item : String) -> Expr {
    match item.parse::<i64>() {
        Ok(n) => Expr::Int(n),
        Err(err) => match item.parse::<f64>() {
            Ok(f) => Expr::Float(f),
            Err(err) => panic!("Not proper number type {}", item)
        },
    }
}

fn parse_item(item : String) -> Expr {
    let c : char = item.chars().next().unwrap();
    match c {
        '\"' => Expr::Str(item),
        //n if n.is_digit() => parse_number(item),
        '0'...'9' => parse_number(item),
        _ => Expr::Symbol(item),
    }
}

fn parse(src : &str) -> (Vec<Expr>, usize) {
    //let mut res : LinkedList<Box<Expr>> = LinkedList::new();
    let mut res = vec![];
    let mut partial = String::new();
    let mut idx : usize = 0;

    for (i, c) in src.chars().enumerate() {
        if i >= idx {
            if c == ' ' {
                if !partial.is_empty() {
                    //println!("<  {}", partial);
                    res.push(parse_item(partial));
                    partial = String::new();
                }
            } else if c == '(' {
                if !partial.is_empty() {
                    //println!("<( {}", partial);
                    res.push(parse_item(partial));
                    partial = String::new();
                }
                let (_, last) = src.split_at(i + 1);
                let (mut a, b) = parse(last);
                idx = i + b + 1;
                let head : Expr = a.swap_remove(0);
                let tail : Vec<_> = a.drain(1..).collect();
                //let (head, tail) = a.split_at(1);
                res.push(Expr::Cons(Box::new(head), Box::new(Expr::List(tail))));
                //res.push(Box::new(a));
            } else if c == ')' {
                if !partial.is_empty() {
                    res.push(parse_item(partial));
                }
                return (res, i + 1);
            } else {
                partial.push(c);
            }
        }
    }

    if !partial.is_empty() {
        res.push(parse_item(partial));
    }
    return (res, idx);
}

fn main() {
    let program = "(begin (define r 10) (* pi (* r r)))";
    let (parsed, _) = parse(program);
    println!("{}", program);
    //parsed.walk_tree(0);
}
