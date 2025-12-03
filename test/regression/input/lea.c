int leaBISD(int base, int index) {
    int scale = 8;
    int displacement = 40;

    int a = base + (index * scale) + displacement;
    int b = displacement + (index * scale) + base;
    int c = (index * scale) + base + displacement;
    int d = (index * scale) + displacement + base;
    int e = base + displacement + (index * scale);
    int f = displacement + base + (index * scale);
    int g = base + (scale * index) + displacement;
    int h = displacement + (scale * index) + base;
    int i = (scale * index) + base + displacement;
    int j = (scale * index) + displacement + base;
    int k = base + displacement + (scale * index);
    int l = displacement + base + (scale * index);

    return 0;
}
