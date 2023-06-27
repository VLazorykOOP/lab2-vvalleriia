#include <bitset>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

void MenuTask() {
    cout << "     Menu Task   \n";
    cout << "    1.  Exersice 1  \n";
    cout << "    2.  Exersice 2 \n";
    cout << "    3.  Exit \n";
}

/*Задано цілі числа a, b, c та d. Обчислити вираз без використання операцій множення та
ділення(замінивши на їх операцій зсувів).*/

int task1() {
    
    int a, b, c, d, x, y;
    cout << "Expression : x = 37*b + (d*31 + 12*a)/2048 - 65*c + d*14\n";
    cout << "Input a,b,c,d:" << endl;

    cin >> a >> b >> c >> d;//розраховує значення змінних
    auto st = cin.rdstate();//присвоюють стан вводу

    if (st != cin.goodbit) { cin.clear(); cout << " error \n"; cin.get(); cin.get(); return 0; }//перевіряємо стан вхідного потоку 
    x = b + (b << 5 + b << 2) + ((d << 5) - d) + (a << 3 + a << 2) >> 11 - c + (c << 6) + (d << 4 - d);
    y = b * 37 + (d * 31 + 12 * a) / 2048 - 65 * c + d * 14;
    cout << "x=" << x << "  y=" << y << endl;
    cin.get();
    cin.get();
    return 0;
}


/*Задано текст до 128 символів. Доповнити пробілами до 128 символів. Шифрувати тексти
таким чином, щоб кожний символ тексту записувався у два байти. Два байти мають таку
структуру:

у бітах 0-3 старша частина ASCII - коду символу (4 біти),
у бітах 4-10 позиція символу в рядку (7 біти),
у бітах 11-14 молодша частина ASCII - коду символу (4 біти),
15 біт – біт парності полів (1 біт).*/

void MyEncryption(char InS[32], unsigned short OutCoding[32], int row) {
    unsigned char c;
    unsigned short r, t, i, b;
    short j;
    for (i = 0; i < 32; i++)
    {
        r = 0;
        c = InS[i];
        t = c;
        r |= t >> 4;
        r |= i << 4;
        t = c;
        r |= t << 11;
        t = 1;
        b = 0;
        for (j = 0; j < 16; j++)
        {
            if (r & t) {
                if (b == 0) b = 1; else b = 0;
            }
            t <<= 1;
        }
        r |= b << 15;
        OutCoding[i] = r;
    }
}

void MyDecryption(char OutS[32], unsigned short InCoding[32]) {
    unsigned char c;
    unsigned short r, t, i, b, p, w;
    short j;
    for (i = 0; i < 32; i++) //
    {
        // Перевірка парності
        r = InCoding[i];

        t = r & 0b1111111111111111; //  0xf7ff      1111 0111 1111
        //  1111
        p = r & 0b0000100000000000; //  0x0800      0000 1000 0000
        //  0000
        w = 1;
        b = 0;
        for (j = 0; j < 16; j++) // обчислення біта парності
        {
            if (t & w) {
                if (b == 0)
                    b = 1;
                else
                    b = 0;
            }
            w <<= 1;
        }
        p >>= 11;
        t = r & 0b1110000000000000; // 0xf000
        t >>= 13;
        w = r & 0b0001111000000000; // 0x000f
        w <<= 3;
        t |= w;
        p = r & 0b0000000111111110; // 0x07f 
        p >>= 1;
        OutS[i] = (unsigned char)p;
    }
}


void task2() {
    // Шифрування даних з використання побітових операцій
    // Data encryption using bitwise operations

    char choice;
    cout << " Data encryption using bitwise operations  \n";
    char S2[4][32] = { '\0' };
    unsigned short Rez[32];
    unsigned short i, f;
    cout << "1. Enter the into bin file \n";
    cout << "2. Read from bin file \n";
    cin >> choice;

    switch (choice) {
    case '1': {
        cout << " Input string (size <=32) \n";
        for (int i = 0; i < 4; i++) {
            cin >> S2[i];
        }
        ofstream ofsb("outb.bin", ios::app | ios::binary);
        for (int i = 0; i < 4; i++) {
            int n = strlen(S2[i]);
            cout << S2[i] << endl;
            MyEncryption(S2[i], Rez, i);
            ofsb.write((char*)Rez, 32 * sizeof(unsigned short));
            cout << "Data write to outb.bin " << endl;
            for (int i = 0; i < n; i++) {
                bitset<16> x(Rez[i]);
                cout << x << '\n';
            };

            for (int i = 0; i < 32; i++) {
                Rez[i] = '\0';
            }

            cout << endl;
        }
        ofsb.close();
        break;
    }
    case '2': {
        char S[32]{ '\0' };
        ofstream ofs("out.txt");
        if (!ofs) {
            cout << "File out.txt not open" << endl;
            return;
        }
        ifstream ifsb("outb.bin", ios::in | ios::binary);
        if (!ifsb) {
            cout << "File outb.bin not open" << endl;
            return;
        }
        for (int i = 0; i < 4; i++) {
            unsigned short InBin[32]{ '\0' };
            if (i) {
                ifsb.seekg(64 * i);
            }
            ifsb.read((char*)InBin, 32 * sizeof(unsigned short));

            MyDecryption(S, InBin);
            cout << "String  " << S << endl;
            ofs << S << endl;
        }
        ifsb.close();
        break;
    }
    default:
        break;
    }
}
int main() {
    cout << "OOP. Template for laboratory work #2.\n";

    char ch = '5';
    do {
        system("cls");
        MenuTask();
        ch = cin.get();

        cin.get();

        switch (ch) {
        case '1':
            task1();
            return 0;
            break;
        case '2':
            task2();
            return 0;
            break;
        }
        cout << " Press any key and enter\n";
        ch = cin.get();
    } while (ch != '2');

    return 0;
}
