int func1(int in1, int in2, int in3, int in4, int in5, int in6, int in7, int in8, int in9, int in10, int in11, int *out1, int *out2, int *out3) {
    //int in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11;
    int a, d, g;
    //int out1, out2, out3;
    a = in1 + in2;
    *out1 = (a - in3) * 3;
    *out2 = in4 + in5 + in6;
    d = in7 * in8;
    g = d + in9 + in10;
    *out3 = in11 * 5 * g;
    return 0;
}
