#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int MAX = 255;

int count;

/*double cost(float w, float b)
{
    double result = 0, y_p;
    for (int i = 0; i < COUNT; i++)
    {
        y_p = w * train[i][0] + b;
        float t =y_p - train[i][1];
        result+=t*t;
    }
    double tmp = result/COUNT;
    return tmp;
}
*/

void gradiant(double w, double b, int train[count][2], int tmp[2])
{
    double dw = 0;
    double db = 0;
    double x,y,y_p;


    for (int i =0; i < count; i++)
    {
        x = (train[i][0]);
        y = (train[i][1]);
        y_p = fmin(ceil(w * x  + b), 255);
        y_p = fmax(y_p, 0);
        double dj_dw_i = (y_p - y) * x;
        double dj_db_i = y_p - y;
        dw+= dj_dw_i;
        db+=dj_db_i;
    }
    tmp[0] = dw/count; //dw
    tmp[1] = db/count; //dw
}


float* linear_regression_model(int c, int train[c][2])
{
    count = c;
    srand(10);
    double w=5 ;
    double b = 0;
    int epoch = 300;
    //double h = 1e-5;
    double rate = 1e-4;

/*    
    for (int i = 0; i < epoch; i++)
    {
        float acc = cost(w,b);
        double dw = (cost(w + h, b) - acc);
        double db = (cost(w , b+h) - acc);

        w -= dw;
        b -= db;
    }
*/    

    int tmp[2];

    for (int i = 0; i < epoch; i++)
    {
        gradiant( w,b, train, tmp);
        w -= tmp[0] *rate ; //tmp[0] is dw
        b -= tmp[1] *rate; //tmp[0] is db
    }

    float *wb = (float*)malloc(2*sizeof(int));
    wb[0] = w;
    wb[1]= b;
    printf("--\n");
    return wb;

}
