#include "iostream"
using namespace std;

//long long approxToInteger(double num) {
long approxToInteger(double num) {
    long result = 0;
    if( num >= 0 ) {
        result = double(double(num) + 0.5);
    } else {
        result = double(double(num) - 0.5);
    }
    return result;
}

class Matrix {
    public:
        int row;
        int col;
        double data[10][10];
        void print();
        bool sameSize(Matrix mat2) {return row == mat2.row && col == mat2.col;};
        Matrix addMatrix(Matrix mat2);
        Matrix subtractMatrix(Matrix mat2);
        Matrix multiplyMatrix(Matrix mat2);
        Matrix divideMatrix(Matrix mat2);
        Matrix inverse();
        long determinant();
        void inputData();

        Matrix(int _row, int _col) { // programmatical input
            row = _row;
            col = _col;
        }


};

void Matrix::inputData() {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cin >> data[i][j];
        }
    }
}

void Matrix::print() {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout << long(approxToInteger(data[i][j]) + 0.0) << " ";
        }
        cout << '\n';
    }
}

Matrix Matrix::addMatrix(Matrix mat2) {
    Matrix result(row, col);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            result.data[i][j] = data[i][j] + mat2.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::subtractMatrix(Matrix mat2) {
    Matrix result(row, col);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            result.data[i][j] = data[i][j] - mat2.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::multiplyMatrix(Matrix mat2) {
    Matrix result(row, mat2.col);
    for(int mat1Row = 0; mat1Row < row; mat1Row++) {
        for(int mat2Col = 0; mat2Col < mat2.col; mat2Col++) {
            double sum = 0;
            for(int k = 0; k < col; k++) {
                sum += data[mat1Row][k] * mat2.data[k][mat2Col];
            }
            result.data[mat1Row][mat2Col] = sum;
        }
    }
    return result;
}

long Matrix::determinant() {
    //cout << "det row: " << row << '\n';
    if(row == 1) {
        return data[0][0];
    }
    else if(row == 2) {
        return (data[0][0] * data[1][1]) - (data[0][1] * data[1][0]);
    } else {
        double result = 0;
        for(int i = 0; i < row; i++) {
            // row 0 will always be hidden
            // hidden column will move from col (0) to col (col-1)
            Matrix newMat(row - 1, col - 1);
            for(int row2 = 1; row2 < row; row2++) {
                for(int col2 = 0; col2 < col; col2++) {
                    if(col2 == i) continue;
                    if(col2 > i) {
                        // we skipped an index in col2 due to it being cancelled by i
                        // so now we adjust by putting (col2 - 1) in the new col index.
                        newMat.data[row2 - 1][col2 - 1] = data[row2][col2];
                    } else {
                        newMat.data[row2 - 1][col2] = data[row2][col2];
                    }
                }
            }
            /*cout << "\nnewMat: (coeff: "<<data[0][i]<<" )\n";
            newMat.print();
            cout << "this result is: "<<newMat.determinant()<<" * "<<data[0][i]<<" = "<<newMat.determinant() * data[0][i];
            */
            if(i % 2 == 0) result += newMat.determinant() * data[0][i];
            else result -= newMat.determinant() * data[0][i];
        }
        return result;
    }
}

Matrix Matrix::inverse() {
    // Using the method of Minors, Cofactors and Adjugate
    // reference: https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html
    // First, getting the Matrix of Minors
    Matrix matrixMinors(row,col);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {

            Matrix newMat(row - 1, col - 1);
            for(int row2 = 0; row2 < row; row2++) {
                for(int col2 = 0; col2 < col; col2++) {
                    if(row2 == i || col2 == j) continue;
                    int newRow = row2;
                    int newCol = col2;
                    if(row2 > i) newRow = row2 - 1;
                    if(col2 > j) newCol = col2 - 1;
                    newMat.data[newRow][newCol] = data[row2][col2];
                }
            }
            matrixMinors.data[i][j] = newMat.determinant();
        }
    }

    // Cofactors
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if((i + j) % 2 == 1) {
                matrixMinors.data[i][j] *= -1;
            }
        }
    }

    // Adjugate / Adjoint
    Matrix adjugate(row, col);

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            adjugate.data[i][j] = matrixMinors.data[j][i];
        }
    }

    // multiply by 1/Determinant
    double determ = determinant();

    Matrix result(row,col);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            result.data[i][j] = adjugate.data[i][j] / double(determ);
        }
    }

    return result;
}

Matrix Matrix::divideMatrix(Matrix mat2) {
    Matrix inversed = mat2.inverse();
    Matrix result = multiplyMatrix(inversed);

    return result;
}

int main() {
    int row1, col1, row2, col2;
    cout << "Please enter dimensions of Matrix A:" << '\n';
    cin >> row1 >> col1;
    cout << "Please enter dimensions of Matrix B:" << '\n';
    cin >> row2 >> col2;
    Matrix mat1(row1, col1);
    Matrix mat2(row2, col2);
    cout << "Please enter values of Matrix A:" << '\n';
    mat1.inputData();
    cout << "Please enter values of Matrix B:" << '\n';
    mat2.inputData();

    while(1) {
        const string errorMsg = "The operation you chose is invalid for the given matrices.\n";
        cout << "Please choose operation type(1: A+B, 2: A-B, 3: AxB, 4: A*inverse(B), 5: |A|, 6: |B|, 7: quit):" << "\n";
        int op;
        cin >> op;
        switch(op) {
        case 1:
            if(mat1.sameSize(mat2)) {
                Matrix result = mat1.addMatrix(mat2);
                result.print();
            } else {
                cout << errorMsg;
            }
            break;
        case 2:
            if(mat1.sameSize(mat2)) {
                Matrix result = mat1.subtractMatrix(mat2);
                result.print();
            } else {
                cout << errorMsg;
            }
            break;
        case 3:
            if(mat1.col == mat2.row) {
                Matrix result = mat1.multiplyMatrix(mat2);
                result.print();
            } else {
                cout << errorMsg;
            }
            break;
        case 4:
            if(mat1.col == mat2.row && mat2.determinant() != 0) {
                Matrix result = mat1.divideMatrix(mat2);
                result.print();
            } else {
                cout << errorMsg;
            }
            break;
        case 5:
            if(mat1.col == mat1.row && mat1.row > 1) {
                cout << mat1.determinant() << '\n';
            } else {
                cout << errorMsg;
            }
            break;
        case 6:
            if(mat2.col == mat2.row && mat2.row > 1) {
                cout << mat2.determinant() << '\n';
            } else {
                cout << errorMsg;
            }
            break;
        case 7:
            cout << "Thank you!";
            return 0;
        }
    }
