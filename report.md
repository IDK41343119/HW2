# 41343119
作業二
## 解題說明


### 解題策略
想法（How to do?）：
  - 用 Term{coef, exp} 的動態陣列 termArray 表示多項式，newTerm 將項 append（遇滿自動擴充）。
  - 加法 Add：把兩個多項視合併；相同次方則係數相加。
  - 乘法 Mult：對 A 的每項與 B 的每項相乘，將乘出項放入結果 c；若 c 已有相同次方則累加，否則新增。最後刪除係數為 0 的項並將結果排序成降冪。
  - Eval：對每項計算 coef * x^exp（使用 powf）並累加。
    
簡短範例：
  - A = $3X^4 + 2X^2 + 1$
  - B = $1X^3 + 4X^1$
  - A+B = $3X^4 + 1X^3 + 2X^2 + 4X^1 + 1$
  - A*B = $3X^7 + 14X^5 + 9X^3 + 4X^1$
  - Eval(2)：$A(2)=57, B(2)$

## 程式實作

以下為主要程式碼：
```cpp
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
```
## 效能分析
假設兩多項各有 n、m 項

時間複雜度：
- Add：O(n + m)
- Mult：兩重迴圈產生 nm 個乘項；每個乘項在結果 c 中做線性搜尋是否同次方（最壞情況 c 可接近 nm），加上結果最後用簡單選擇式排序 O(c^2)。總體最壞可達 O((n*m)^2)（或寫作 O(n^2 m^2) 的上界），實務上隨項數增大會非常慢。
- Eval：O(k)（k 為該多項的項數），每項呼叫 powf。

空間複雜度：
- 結果多項 c 最多可能有 O(nm) 項（每對項次方不同），因此額外空間為 O(nm)。整體記憶體由 termArray 與結果佔用，且使用原生 new[]/delete[] 管理。

## 測試與驗證

### 測試案例
<img width="561" height="217" alt="image" src="https://github.com/user-attachments/assets/dcf2f100-6597-43da-be75-630af740d650" />
<img width="776" height="226" alt="image" src="https://github.com/user-attachments/assets/d9a2e343-cccc-4be5-b81d-fc50b7e47fcd" />


## 效能測量
預期Add 應呈線性增長，Mult 隨 n,m 增大會呈現快速上升。

## 心得
這次在寫程式時第一次碰到double-delete的問題，後來學習到能透過深複製來解決，也加強了我對於類別的熟悉度，以及練習了多載，不得不說這次的做起來難度比上次高了不少。

