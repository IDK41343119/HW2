# 41343119
作業二
## 解題說明


### 解題策略
- 第一題:
  - 遞迴
    1. 把題目分解成 $m = 0$ , $n = 0$ ,m跟n都不為0的情況，並組合成遞迴函式。
    2. 當 $m = 0$ 時，返回 $n+1$ 作為遞迴的結束條件。
  - 非遞迴
    1. 用陣列模擬遞迴呼叫
    2. 不斷從堆疊中取出一個 m 來處理
    3. 根據 m 和 n 的值決定下一步動作：
  
- 第二題:
  - 用遞迴來決定第 index 個元素要不要放進子集合，直到 index == n（走到最後），就輸出目前結果。
  - 對於每一個元素，有兩種選擇：
    1. 不選它
    → 子集合不包含這個元素
    2. 選它
    → 子集合包含這個元素

## 程式實作

以下為主要程式碼：
- 第一題
```cpp
#include <iostream>
#include <cstdlib> // for exit()

using namespace std;

// 遞迴版
int AckermannRecursive(int m, int n) {
    if (m == 0) {
        return n + 1;
    }
    else if (n == 0) {
        return AckermannRecursive(m - 1, 1);
    }
    else {
        return AckermannRecursive(m - 1, AckermannRecursive(m, n - 1));
    }
}

//非遞迴版

int AckermannNonRecursive(int m, int n) {
    int stack[100000];
    int top = 0;

    stack[top++] = m;

    while (top > 0) {
        m = stack[--top]; // pop

        if (m == 0) {
            n = n + 1;
        }
        else if (n == 0) {
            stack[top++] = m - 1;
            n = 1;
        }
        else {
            stack[top++] = m - 1;
            stack[top++] = m;
            n = n - 1;
        }
    }

    return n;
}

//主程式
int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;
    cout << "[遞迴] Ackermann(" << m << ", " << n << ") = " << AckermannRecursive(m, n) << endl;
    cout << "[非遞迴] Ackermann(" << m << ", " << n << ") = " << AckermannNonRecursive(m, n) << endl;
    return 0;
}

```
- 第二題
```cpp
#include <iostream>
#include <string>
using namespace std;

// 遞迴列印 powerset
void powerSet(const string S[], int n, int index, string subset) {
    if (index == n) {
        cout << "{ " << subset << " }";
        return;
    }

    // 不包含目前元素
    powerSet(S, n, index + 1, subset);

    // 包含目前元素
    string next = subset;
    if (!next.empty()) next += " ";
    next += S[index];
    powerSet(S, n, index + 1, next);
}

int main() {
    int n;
    cout << "輸入集合的元素個數 n: ";
    cin >> n;

    if (n <= 0) {
        cout << "集合為空。" << endl;
        return 0;
    }

    string S[100];  // 假設最多 100 個元素
    cout << "請輸入 " << n << " 個元素（用空白分隔）:" << endl;

    for (int i = 0; i < n; i++) {
        cin >> S[i];
    }

    cout << "\n集合的冪集 (powerset) 為：" << endl;
    powerSet(S, n, 0, "");

    return 0;
}

```
## 效能分析
- 第一題($m \leq 3$)

| m 值 | 時間複雜度 | 空間複雜度 |
| --- | ----- | ----- |
| 0   | O(1)  | O(1)  |
| 1   | O(n)  | O(n)  |
| 2   | O(n)  | O(n)  |
| 3   | O(2ⁿ) | O(n)  |

- 第二題
1. 時間複雜度：程式的時間複雜度為 $O(2^n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例
- 第一題

| 測試案例 | 輸入參數 $m$ | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|--------------|----------|-----------|
| 測試一   | $m = 1$      | $n = 10$     | 12        | 遞迴:12   非遞迴:12      |
| 測試二   | $m = 2$      | $n = 5$      | 13        | 遞迴:13   非遞迴:13      |
| 測試三   | $m = 3$      | $n = 2$      | 29        | 遞迴:29   非遞迴:29      |
| 測試四   | $m = 4$      | $n = 1$      | 65533     | 無法計算   |

- 第二題

| 測試案例 | 輸入參數 $n$ | 輸入集合 $S$ | 預期輸出 | 實際輸出 |
|----------|--------------|--------------|----------|-----------|
| 測試一   | $n = 3$      | $S = {1,2,3}$     | {  }{ 3 }{ 2 }{ 2 3 }{ 1 }{ 1 3 }{ 1 2 }{ 1 2 3 }|{  }{ 3 }{ 2 }{ 2 3 }{ 1 }{ 1 3 }{ 1 2 }{ 1 2 3 }|
| 測試二   | $n = 4$      | $S = {t,e,s,t}$      |{  }{ t }{ s }{ s t }{ e }{ e t }{ e s }{ e s t }{ t }{ t t }{ t s }{ t s t }{ t e }{ t e t }{ t e s }{ t e s t }|{  }{ t }{ s }{ s t }{ e }{ e t }{ e s }{ e s t }{ t }{ t t }{ t s }{ t s t }{ t e }{ t e t }{ t e s }{ t e s t }|
| 測試三   | $n = -1$      |      | 集合為空        | 集合為空        |
### 結論
第一題
1. 程式能正確計算 ($m$ , $n$) 的Ackermann's function。  
2. 在 $m \gt 3$ 的情況下，程式會Stack Overflow 
3. 在 $m \leq 3$ 的情況下，程式會正確顯示結果

第二題
1. 能夠正確計算冪集
2. 在 $n\leq 0$的情況下，程式會輸出集合為空


## 申論及開發報告

### 第一題非遞迴選擇陣列的原因
- 用能夠用堆疊模仿遞迴
### 第二題選擇遞迴的原因
1. 對於每個元素，有兩個選擇：
  -  不包含這個元素
  -  包含這個元素

「對每個元素有兩個選擇」的結構，天然形成了一個二叉樹，遞迴正好可以用程式自然地表現這種分支結構
  
2. 代碼簡潔、清楚，不需要寫一堆迴圈
