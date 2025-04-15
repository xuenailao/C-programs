#include "TrinomialTree.hpp"
#include <iostream>
#include <chrono> // ������ʱ���ܵ�ͷ�ļ�

using namespace std;

int main() {
    // ������Ȩ����
    double S = 1;       // ��ʼ��Ʊ�۸�
    double K = 2;       // ��Ȩ��
    double r = 0.08;    // �޷�������
    double sigma = 0.2; // ������
    double T = 10;      // ����ʱ�䣨�꣩
    int nsteps = 100;   // ʱ�䲽��

    double lambda = 1;  // Lambda ����

    // ������Ȩ��������ģ��
    Option CALL(S, K, r, sigma, T, true);
    Option PUT(S, K, r, sigma, T, false);
    TrinomialTree TREE1(CALL, nsteps, lambda);
    TrinomialTree TREE2(PUT, nsteps, lambda);

    // ��ʼ��ʱ
    auto start = chrono::high_resolution_clock::now();

    // ���㿴����Ȩ�۸�
    double C = TREE1.priceOption();
    auto endCall = chrono::high_resolution_clock::now(); // ��¼������Ȩ����ʱ��
    cout << "Option Price (Call): " << C << endl;

    // ���㿴����Ȩ�۸�
    double P = TREE2.priceOption();
    auto endPut = chrono::high_resolution_clock::now(); // ��¼������Ȩ����ʱ��
    cout << "Option Price (Put): " << P << endl;

    // ��֤Put-Callƽ��
    double left = S + P - C;
    double right = K * exp(-r * T);
    cout << "S + P - C: " << left << endl;
    cout << "K * e^(-rT): " << right << endl;

    // ��������ʱ��
    auto durationCall = chrono::duration_cast<chrono::milliseconds>(endCall - start);
    auto durationPut = chrono::duration_cast<chrono::milliseconds>(endPut - endCall);
    auto totalDuration = chrono::duration_cast<chrono::milliseconds>(endPut - start);

    // �������ʱ��
    cout << "Time taken for Call Option: " << durationCall.count() << " ms" << endl;
    cout << "Time taken for Put Option: " << durationPut.count() << " ms" << endl;
    cout << "Total Time taken: " << totalDuration.count() << " ms" << endl;

    return 0;
}
