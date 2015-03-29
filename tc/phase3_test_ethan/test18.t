// Class depending on itself
class T extends S {}
class S extends T {} // Error

class A extends B {}
class B extends C {}
class C extends A {} // Error

int main() {}
