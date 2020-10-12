fn main() {
    let mut x = 5;
    println!("The value of x is: {}", x);
    x = 6;
    println!("The value of x is: {}", x);
    let x = b'A';
    println!("The value of x is: {}", x);
    let x = 0b111_0000;
    println!("The value of x is: {}", x);
    let x = 0o77;
    println!("The value of x is: {}", x);
    let x = 98_222;
    println!("The value of x is: {}", x);
    let x = 0xff;
    println!("The value of x is: {}", x);
    let x = 2.0; // f64
    println!("The value of x is: {}", x);
    let y: f32 = 3.0; // f32
    println!("The value of y is: {}", y);
    let t = true;
    println!("The value of t is: {}", t);
    let f: bool = false; // with explicit type annotation
    println!("The value of f is: {}", f);
    let tup = (500, 6.4, 1);
    let (x, y, z) = tup;
    println!("The value of y is: {},{},{}", x, y, z);
    println!("The value of tup is: {},{},{}", tup.0, tup.1, tup.2);
    let a = [3; 5];
    println!("The value of a is: {:?}", a);
    let a: [i32; 5] = [1, 2, 3, 4, 5];
    println!("The value of a is: {:?}", a);
    let a = [
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
    ];
    println!("The value of a is: {:?}", a);

    // function
    another_function(5, 6);

    // statements and expression
    let y = {
        let x = 3;
        x + 1
    };
    println!("The value of y is: {}", y);
    let x = five();
    println!("The value of x is: {}", x);

    let mut s = String::from("hello");

    s.push_str(", world!"); // push_str() appends a literal to a String

    println!("{}", s); // This will print `hello, world!`
}

fn another_function(x: i32, y: i32) {
    println!("The value of x is: {}", x);
    println!("The value of y is: {}", y);
}

fn five() -> i32 {
    5
}
