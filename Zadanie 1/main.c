#include <stdio.h>
#include <math.h>

void row_print(double x, double *f_num) { // printf riadky s formatom
    printf("%10.2lf", x);
    for (int i = 0; i < 6; i++) {
        printf("%10.2lf", f_num[i]);
    }
    printf("\n");
}

double f1(double x) { // Binárna skoková funkcia
    if (x < 0) {
        return 0;
    } else {
        return 1;
    }
}

double f2(double x) { // Sigmoida
    return (1/(1+(pow(M_E, (-1)*x))));
}

double f3(double x) { // Hyperbolický tangens
    return ((pow(M_E, x) - pow(M_E, (-1)*x)) / (pow(M_E, x) + pow(M_E, (-1)*x)));
}

double f4(double x) { // Gaussova funkcia
    return (pow(M_E, ((-1)*x*x)));
}

double f5(double x) { // ReLU funkcia
    if (x <= 0) {
        return 0;
    } else {
        return x;
    }
}

double f6(double x) { // Squareplus funckia
    return (0.5*(x + sqrt(x*x + 4)));
}

void automatic_mode() { // input 0 = 0
    double x_start;
    double x_end;
    double step;
    if (scanf("%lf", &x_start) && scanf("%lf", &x_end) && scanf("%lf", &step)) {
        if (x_start < x_end) {
            do {
                double f_num[6] = {f1(x_start), f2(x_start), f3(x_start), f4(x_start), f5(x_start), f6(x_start)};
                row_print(x_start, f_num);
                x_start = x_start + step;
            } while (x_start <= x_end);
        } else {
            printf("E3\n");
        }
    } else {
        printf("E2\n");
    }
}

void manual_mode() { // input 0 = 1
    double i_x;
    while (scanf("%lf", &i_x)) {
        double f_num[6] = {f1(i_x), f2(i_x), f3(i_x), f4(i_x), f5(i_x), f6(i_x)};
        row_print(i_x, f_num);
    }
}

int main() {
    int mode_select;
    if (scanf("%d", &mode_select)) {
        if (mode_select == 0) {
            automatic_mode();
        } else if (mode_select == 1) {
            manual_mode();
        } else {
            printf("E1\n");
        }
    } else {
        printf("E1\n");
    }
}
