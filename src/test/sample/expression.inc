print(1);
print(1.3);
a = 3;
a = 3333.3;
print(a, 3.0);

func add(a, b) {
    a + b;
}

print(3.0 / 3);
a = add(3.0, 3);
print(a);

print(true);
print(1 <= -1.0 + 1);
print(true || false);
print(false || false);
print(false && false);
print(true && false);
print(true && true);

print("start test of if statement");
if (false) {
    print(true);
} else if (false) {
    print(false);
} else {
    print("c");
}

func fib(n) {
    if (n <= 2) {
        1;
    } else {
        break;
        fib(n - 1) + fib(n - 2);
    }
}

func qiuhe(n) {
    n * (n + 1) / 2;
}

print("fib answer: ", fib(3), "qiuhe ansower: ", qiuhe(10));

index = 0;
while (index < 100) {
    index = index + 1;
    print(index);
}

for (j = 0; j <= 1; j = j + 1) {
    print("j: ", j);
}
