
#include "stats.h"

stats::stats(PNG& im) {
    /* Your code here!! */

    for (unsigned int i = 0; i < im.width(); i++) {
        vector<long> v;
        sumRed.push_back(v);
        sumGreen.push_back(v);
        sumBlue.push_back(v);
        sumsqRed.push_back(v);
        sumsqGreen.push_back(v);
        sumsqBlue.push_back(v);
        for (unsigned int j = 0; j < im.height(); j++) {
            sumGreen[i].push_back(0);
            sumRed[i].push_back(0);
            sumBlue[i].push_back(0);
            sumsqGreen[i].push_back(0);
            sumsqRed[i].push_back(0);
            sumsqBlue[i].push_back(0);
        }
    }

    for (unsigned int i = 0; i < im.width(); i++) {
        for (unsigned int j = 0; j < im.height(); j++) {
            RGBAPixel* p = im.getPixel(i, j);
            if (i == 0) {
                if (j == 0) {
                    sumRed[i][j] = p->r;
                    sumGreen[i][j] = p->g;
                    sumBlue[i][j] = p->b;
                    sumsqRed[i][j] = pow(p->r, 2);
                    sumsqGreen[i][j] = pow(p->g, 2);
                    sumsqBlue[i][j] = pow(p->b, 2);
                } else {
                    sumRed[i][j] = p->r + sumRed[i][j-1];
                    sumGreen[i][j] = p->g + sumGreen[i][j-1];
                    sumBlue[i][j] = p->b + sumBlue[i][j-1];
                    sumsqRed[i][j] = pow(p->r, 2) + sumsqRed[i][j-1];
                    sumsqGreen[i][j] = pow(p->g, 2) + sumsqGreen[i][j-1];
                    sumsqBlue[i][j] = pow(p->b, 2) + sumsqBlue[i][j-1];
                }
            } else if (j == 0) {
                sumRed[i][j] = p->r + sumRed[i-1][j];
                sumGreen[i][j] = p->g + sumGreen[i-1][j];
                sumBlue[i][j] = p->b + sumBlue[i-1][j];
                sumsqRed[i][j] = pow(p->r, 2) + sumsqRed[i-1][j];
                sumsqGreen[i][j] = pow(p->g, 2) + sumsqGreen[i-1][j];
                sumsqBlue[i][j] = pow(p->b, 2) + sumsqBlue[i-1][j];
            } else {
                sumRed[i][j] = p->r + sumRed[i-1][j] + sumRed[i][j-1] - sumRed[i-1][j-1];
                sumGreen[i][j] = p->g + sumGreen[i-1][j] + sumGreen[i][j-1] - sumGreen[i-1][j-1];
                sumBlue[i][j] = p->b + sumBlue[i-1][j] + sumBlue[i][j-1] - sumBlue[i-1][j-1];
                sumsqRed[i][j] = pow(p->r, 2) + sumsqRed[i-1][j] + sumsqRed[i][j-1] - sumsqRed[i-1][j-1];
                sumsqGreen[i][j] = pow(p->g, 2) + sumsqGreen[i-1][j] + sumsqGreen[i][j-1] - sumsqGreen[i-1][j-1];
                sumsqBlue[i][j] = pow(p->b, 2) + sumsqBlue[i-1][j] + sumsqBlue[i][j-1] - sumsqBlue[i-1][j-1];
            }
        }
    }
}


long stats::getSum(char channel, pair<int, int> ul, int dim) {
        /* Your code here!! */
    long result = 0;
    long sqLength = pow(2,dim)-1; // length of the square (-1 is to make sure last pixel accessed is within square)
    int x = ul.first; // top left x coord of square
    int y = ul.second; // top left y coord of square
    /*the sum of the values in the desired square can be gotten by the value stored in the opposite diagonal
    * this will additionally count the squares surrounding the intended square
    * to handle this, we will subtract the total area above the intended square and the area to the left of the square
    * since this double deletes the squares diagonal to the upper left, these will be addded back
    */ 

    long bigSumRed = sumRed[x+sqLength][y+sqLength];
    long bigSumGreen = sumGreen[x+sqLength][y+sqLength];
    long bigSumBlue = sumBlue[x+sqLength][y+sqLength];

    long smallSumRed = 0;
    long smallSumGreen = 0;
    long smallSumBlue = 0;

    long topSumRed = 0;
    long topSumGreen = 0;
    long topSumBlue = 0;

    long leftSumRed = 0;
    long leftSumGreen = 0;
    long leftSumBlue = 0;

    if(x != 0 && y !=0) {
        //where x and y don't equal 0
        smallSumRed = sumRed[x-1][y-1];
        smallSumGreen = sumGreen[x-1][y-1];
        smallSumBlue = sumBlue[x-1][y-1];

        topSumRed = sumRed[x+sqLength][y-1];
        topSumGreen = sumGreen[x+sqLength][y-1];
        topSumBlue = sumBlue[x+sqLength][y-1];

        leftSumRed = sumRed[x-1][y+sqLength];
        leftSumGreen = sumGreen[x-1][y+sqLength];
        leftSumBlue = sumBlue[x-1][y+sqLength];
    } else {
        //where one of x and y equals 0
        if (x == 0 && y == 0) {
            //where x = 0 and y = 0
            // nothing changes, use initialized values
        }
        else if (x == 0) {
            //where only x = 0
            topSumRed = sumRed[x+sqLength][y-1];
            topSumGreen = sumGreen[x+sqLength][y-1];
            topSumBlue = sumBlue[x+sqLength][y-1];
        } else {
            //where only y = 0
            leftSumRed = sumRed[x-1][y+sqLength];
            leftSumGreen = sumGreen[x-1][y+sqLength];
            leftSumBlue = sumBlue[x-1][y+sqLength];
        }
    }

    if (channel == 'r') {
        result = bigSumRed - topSumRed - leftSumRed + smallSumRed;
    } else if (channel == 'g') {
        result = bigSumGreen - topSumGreen - leftSumGreen + smallSumGreen;
    } else {
        result = bigSumBlue - topSumBlue - leftSumBlue + smallSumBlue;
    }
    return result;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */
    long result = 0;
    long sqLength = pow(2,dim)-1; // length of the square (-1 is to make sure last pixel accessed is within square)
    int x = ul.first; // top left x coord of square
    int y = ul.second; // top left y coord of square
    /*the sum of the values in the desired square can be gotten by the value stored in the opposite diagonal
    * this will additionally count the squares surrounding the intended square
    * to handle this, we will subtract the total area above the intended square and the area to the left of the square
    * since this double deletes the squares diagonal to the upper left, these will be addded back
    */ 
    long bigSumsqRed = sumsqRed[x+sqLength][y+sqLength];
    long bigSumsqGreen = sumsqGreen[x+sqLength][y+sqLength];
    long bigSumsqBlue = sumsqBlue[x+sqLength][y+sqLength];

    long smallSumsqRed = 0;
    long smallSumsqGreen = 0;
    long smallSumsqBlue = 0;

    long topSumsqRed = 0;
    long topSumsqGreen = 0;
    long topSumsqBlue = 0;

    long leftSumsqRed = 0;
    long leftSumsqGreen = 0;
    long leftSumsqBlue = 0;

    if(x != 0 && y !=0) {
        //where x and y don't equal 0
        smallSumsqRed = sumsqRed[x-1][y-1];
        smallSumsqGreen = sumsqGreen[x-1][y-1];
        smallSumsqBlue = sumsqBlue[x-1][y-1];

        topSumsqRed = sumsqRed[x+sqLength][y-1];
        topSumsqGreen = sumsqGreen[x+sqLength][y-1];
        topSumsqBlue = sumsqBlue[x+sqLength][y-1];

        leftSumsqRed = sumsqRed[x-1][y+sqLength];
        leftSumsqGreen = sumsqGreen[x-1][y+sqLength];
        leftSumsqBlue = sumsqBlue[x-1][y+sqLength];
    } else {
        //where one of x and y equals 0
        if (x == 0 && y == 0) {
            //where x = 0 and y = 0
            // nothing changes, use initialized values
        }
        else if (x == 0) {
            //where only x = 0
            topSumsqRed = sumsqRed[x+sqLength][y-1];
            topSumsqGreen = sumsqGreen[x+sqLength][y-1];
            topSumsqBlue = sumsqBlue[x+sqLength][y-1];
        } else {
            //where only y = 0
            leftSumsqRed = sumsqRed[x-1][y+sqLength];
            leftSumsqGreen = sumsqGreen[x-1][y+sqLength];
            leftSumsqBlue = sumsqBlue[x-1][y+sqLength];
        }
    }

    if (channel == 'r') {
        result = bigSumsqRed - topSumsqRed - leftSumsqRed + smallSumsqRed;
    } else if (channel == 'g') {
        result = bigSumsqGreen - topSumsqGreen - leftSumsqGreen + smallSumsqGreen;
    } else {
        result = bigSumsqBlue - topSumsqBlue - leftSumsqBlue + smallSumsqBlue;
    }
    return result;
}


long stats::rectArea(int dim) {
    /* Your code here!! */
    long sqLength = pow(2,dim); // length of the square (-1 is to make sure last pixel accessed is within square)
    return pow(sqLength, 2);
}


// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    /* Your code here!! */
    long area = rectArea(dim);

    long sumRed = getSum('r', ul, dim);
    long sumGreen = getSum('g', ul, dim);
    long sumBlue = getSum('b', ul, dim);

    long sumsqRed = getSumSq('r', ul, dim);
    long sumsqGreen = getSumSq('g', ul, dim);
    long sumsqBlue = getSumSq('b', ul, dim);

    double ssdRed = sumsqRed - pow(sumRed, 2)/area;
    double ssdGreen = sumsqGreen - pow(sumGreen, 2)/area;
    double ssdBlue = sumsqBlue - pow(sumBlue, 2)/area;

    return ssdRed + ssdGreen + ssdBlue;
}


RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    /* Your code here!! */
    int area = rectArea(dim);

    int sumRed = getSum('r', ul, dim);
    int avgRed = sumRed/area;

    int sumGreen = getSum('g', ul, dim);
    int avgGreen = sumGreen/area;

    int sumBlue = getSum('b', ul, dim);
    int avgBlue = sumBlue/area;

    RGBAPixel p(avgRed, avgGreen, avgBlue);
    return p;
}
