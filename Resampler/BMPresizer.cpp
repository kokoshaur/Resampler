#include <fstream>
#include <vector>

#include "bmp.h"
using namespace std;
class BMPresizer
{
private:
	string pathToInp;
    string pathToOut;

    ifstream file;
    ofstream outfile;
	
    int sizeRow = 3;
    int sizeColumn = 3;
    int newRow;
    int newColumt;

    vector<vector<char>> subj;
    vector<vector<double>> bufRow;
    vector<vector<double>> bufColumn;

public:
    BMPresizer(string pathToInp = "D:\\vs\\progect\\SignalHandlerTools\\data.bmp", string pathToOut = "D:\\vs\\progect\\SignalHandlerTools\\data2.bmp")
    {
        this->pathToInp = pathToInp;
        this->pathToOut = pathToOut;

        file = ifstream(pathToInp, ios::binary | ios::out);
        outfile = ofstream(pathToOut, ios::binary | ios::out);
    }
	
    void resize(double n, double m) {
        if (!file || !outfile)
            return;
    	
        init(n, m);
        doMagic();
        finish();
    }

private:
	void init(double n, double m)
	{
		vector<char> head(sizeof(BITMAPFILEHEADER) +sizeof(BITMAPINFOHEADER));
        file.read(&head[0], head.size());

        sizeRow = 3 * ((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biWidth;
        sizeColumn = ((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biHeight + 1;
		
        newRow = aga(sizeRow * n);
        newColumt = aga(sizeColumn * m);

        subj = newMatrix(subj, sizeColumn, sizeRow);
        bufRow = newMatrix(bufRow, sizeColumn, newRow);
        bufColumn = newMatrix(bufColumn, newRow, newColumt);
		
        ((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biWidth = (LONG)aga(((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biWidth * n);
        ((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biHeight = (LONG)aga(((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biHeight * m);
        ((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biSizeImage = (LONG)aga(((PBITMAPINFOHEADER)(&head[0] + sizeof(BITMAPFILEHEADER)))->biSizeImage * n * m);
        outfile.write(&head[0], head.size());

        for (int i = 0; i < sizeColumn; i++)
            file.read(&subj[i][0], subj[i].size());
	}
	
    template<class Type>
    vector<vector<Type>> newMatrix(vector<vector<Type>> newSubj, int Column, int Row)
	{
        newSubj.resize(Column);
        for (int i = 0; i < Column; i++)
            newSubj[i].resize(Row);

        return newSubj;
	}
	
	void doMagic()
	{
        for (int i = 0; i < sizeColumn; i++)
            bufRow[i] = oversampleng(subj[i], newRow);

        bufRow = transponse(bufRow);

        for (int i = 0; i < newRow; i++)
            bufColumn[i] = oversampleng(bufRow[i], newColumt);

        bufColumn = transponse(bufColumn);

        subj = newMatrix(subj, newColumt, newRow);
		
        for (int i = 0; i < newColumt; i++)
            for (int j = 0; j < newRow; j++)
                subj[i][j] = (char)(bufColumn[i][j]);
	}

    template<class Type>
    vector<double> oversampleng(vector<Type> old, int newFd)
	{
        double newTd = 1.0/newFd;
        double time = 0.0;
        vector<double> buf(newFd);
        buf.resize(newFd);

        for (int i = 0; i < newFd; i++)
            buf[i] = old[aga((i * old.size() / newFd))];

        vector<double> ansver (newFd);

        for (int i = 0; i < newFd; i++)
        {
            ansver[i] = buf[i] * sinc((time - i * newTd) / newTd);
            time += newTd;
        }
		
        return ansver;
	}

	int aga(double x)
	{
        return floor(x);
	}

	double sinc(double x)
	{
        if (x == 0)
            return 1;
        return sin(x) / x;
	}

    vector<vector<double>> transponse(vector<vector<double>>matrix)
	{
        vector<vector<double>> ansver;
        ansver = newMatrix(ansver, matrix[0].size(), matrix.size());
		
        for (int i = 0; i < matrix.size(); ++i)
            for (int j = 0; j < ansver.size(); ++j)
                ansver[j][i] = matrix[i][j];

		return ansver;
	}
	
	void finish()
	{
        for (int i = 0; i < subj.size(); i++)
            outfile.write(&subj[i][0], subj[i].size());
	}
};
