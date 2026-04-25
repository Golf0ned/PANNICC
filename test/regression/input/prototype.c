int foo(int bar);
int bar(int foo);

int use_proto() {
    return foo(1) + bar(2);
}
