#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend Polynomial;
    friend ostream& operator<<(ostream& output, const Polynomial& Poly);
private:
    int exp;
    float coef;
};

class Polynomial {
private:
    Term* termArray;    //陣列本體
    int capacity;       //陣列大小
    int terms;          //陣列內持數字
public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    // 最低限度的深拷貝以避免回傳時 double-delete
    Polynomial(const Polynomial& other) : capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];
        copy(other.termArray, other.termArray + other.terms, termArray);
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        delete[] termArray;
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        copy(other.termArray, other.termArray + other.terms, termArray);
        return *this;
    }

    ~Polynomial() { delete[] termArray; }

    Polynomial Add(Polynomial b);

    Polynomial Mult(Polynomial b);

    float Eval(float x);

    void newTerm(const float newcoef, const int newexp);

    friend istream& operator>>(istream& input, Polynomial& Poly);
    friend ostream& operator<<(ostream& output, const Polynomial& Poly);
};

istream& operator>>(istream& is, Polynomial& poly) {
    float coef;
    int exp, n;
    if (!(is >> n)) return is;
    while (n--) {
        is >> coef >> exp;
        poly.newTerm(coef, exp);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& poly) {
    if (poly.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < poly.terms; ++i) {
        if (i > 0) os << " + ";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}

Polynomial Polynomial::Add(Polynomial b) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < b.terms))
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t) c.newTerm(t, termArray[aPos].exp);
            aPos++; bPos++;
        }
        else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
        else {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }

    for (;aPos < terms;aPos++)
        c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (;bPos < b.terms;bPos++)
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
    return c;
}

Polynomial Polynomial::Mult(Polynomial b) {
    // 逐項相乘，若同指數則累加係數
    Polynomial c;
    for (int i = 0; i < terms; ++i) {
        for (int j = 0; j < b.terms; ++j) {
            int newExp = termArray[i].exp + b.termArray[j].exp;
            float newCoef = termArray[i].coef * b.termArray[j].coef;

            // 如果 c 中已經有相同指數，則累加；否則新增
            bool found = false;
            for (int k = 0; k < c.terms; ++k) {
                if (c.termArray[k].exp == newExp) {
                    c.termArray[k].coef += newCoef;
                    found = true;
                    break;
                }
            }
            if (!found) {
                c.newTerm(newCoef, newExp);
            }
        }
    }

    // 移除係數為 0 的項（可能是累加後變為 0）
    int write = 0;
    for (int i = 0; i < c.terms; ++i) {
        if (c.termArray[i].coef != 0.0f) {
            if (write != i) c.termArray[write] = c.termArray[i];
            ++write;
        }
    }
    c.terms = write;

    //把結果依 exp 做降冪排序
    for (int i = 0; i < c.terms; ++i) {
        int maxIdx = i;
        for (int j = i + 1; j < c.terms; ++j) {
            if (c.termArray[j].exp > c.termArray[maxIdx].exp) maxIdx = j;
        }
        if (maxIdx != i) swap(c.termArray[i], c.termArray[maxIdx]);
    }

    return c;
}

float Polynomial::Eval(float x) {
    float result = 0.0f;
    for (int i = 0; i < terms; ++i) {
        result += termArray[i].coef * powf(x, termArray[i].exp);
    }
    return result;
}

void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[]termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms++].exp = theExp;
}

int main() {
    // 輸入格式：
    // n coef exp coef exp ...  (n 為項數)
    
    Polynomial A, B;
    if (!(cin >> A)) return 0; // 若沒有輸入則結束
    if (!(cin >> B)) return 0;

    cout << "A = " << A << endl;
    cout << "B = " << B << endl;

    Polynomial sum = A.Add(B);
    cout << "A + B = " << sum << endl;

    Polynomial prod = A.Mult(B);
    cout << "A * B = " << prod << endl;

    // 若接著還有一個 float，則對 A 與 B 做 evaluation
    float x;
    if (cin >> x) {
        cout << "A(" << x << ") = " << A.Eval(x) << endl;
        cout << "B(" << x << ") = " << B.Eval(x) << endl;
    }

    return 0;
}