#ifndef SAMPLETOTEMPERATURE_H_
#define SAMPLETOTEMPERATURE_H_

// 映射表长度1000
#define TABLE_SIZE 1000

class TemperTable
{
public:
    TemperTable()
    {
        initTable();
    }

    ~TemperTable()
    {}

    int getTemperature(int sample)
    {
        if (sample < 52 || sample > 959)
        {
            return -1000;
        }
        else
        {
            return _temperTable[sample];
        }

    }

    void initTable()
    {
        memset(_temperTable,0,TABLE_SIZE*sizeof(int));
//        for (int i = 52;i<960;++i)
//        {
//            if (i<56)
//            {
//                _temperTable[i] = -30;
//                continue;
//            }
//            else if (i < 59)
//            {
//                _temperTable[i] = -29;
//                continue;
//            }
//            else if (i < 63)
//            {
//                _temperTable[i] = -28;
//                continue;
//            }
//            else if (i < 66)
//            {
//                _temperTable[i] = -27;
//                continue;
//            }
//            else if (i < 71)
//            {
//                _temperTable[i] = -26;
//                continue;
//            }
//            else if (i < 75)
//            {
//                _temperTable[i] = -25;
//                continue;
//            }
//            else if (i < 78)
//            {
//                _temperTable[i] = -24;
//                continue;
//            }
//            else if (i < 83)
//            {
//                _temperTable[i] = -23;
//                continue;
//            }
//            else if (i < 87)
//            {
//                _temperTable[i] = -22;
//                continue;
//            }
//            else if (i < 92)
//            {
//                _temperTable[i] = -21;
//                continue;
//            }
//            else if (i < 97)
//            {
//                _temperTable[i] = -20;
//                continue;
//            }
//            else if (i < 103)
//            {
//                _temperTable[i] = -19;
//                continue;
//            }
//            else if (i < 108)
//            {
//                _temperTable[i] = -18;
//                continue;
//            }
//            else if (i < 114)
//            {
//                _temperTable[i] = -17;
//                continue;
//            }
//            else if (i < 119)
//            {
//                _temperTable[i] = -16;
//                continue;
//            }
//            else if (i < 126)
//            {
//                _temperTable[i] = -15;
//                continue;
//            }
//            else if (i < 132)
//            {
//                _temperTable[i] = -14;
//                continue;
//            }
//            else if (i < 139)
//            {
//                _temperTable[i] = -13;
//                continue;
//            }
//            else if (i < 146)
//            {
//                _temperTable[i] = -12;
//                continue;
//            }
//            else if (i < 153)
//            {
//                _temperTable[i] = -11;
//                continue;
//            }
//            else if (i < 160)
//            {
//                _temperTable[i] = -10;
//                continue;
//            }
//            else if (i < 167)
//            {
//                _temperTable[i] = -9;
//                continue;
//            }
//            else if (i < 175)
//            {
//                _temperTable[i] = -8;
//                continue;
//            }
//            else if (i < 183)
//            {
//                _temperTable[i] = -7;
//                continue;
//            }
//            else if (i < 191)
//            {
//                _temperTable[i] = -6;
//                continue;
//            }
//            else if (i < 199)
//            {
//                _temperTable[i] = -5;
//                continue;
//            }
//            else if (i < 208)
//            {
//                _temperTable[i] = -4;
//                continue;
//            }
//            else if (i < 217)
//            {
//                _temperTable[i] = -3;
//                continue;
//            }
//            else if (i < 226)
//            {
//                _temperTable[i] = -2;
//                continue;
//            }
//            else if (i < 235)
//            {
//                _temperTable[i] = -1;
//                continue;
//            }
//            else if (i < 245)
//            {
//                _temperTable[i] = 0;
//                continue;
//            }
//            else if (i < 255)
//            {
//                _temperTable[i] = 1;
//                continue;
//            }
//            else if (i < 265)
//            {
//                _temperTable[i] = 2;
//                continue;
//            }
//            else if (i < 275)
//            {
//                _temperTable[i] = 3;
//                continue;
//            }
//            else if (i < 285)
//            {
//                _temperTable[i] = 4;
//                continue;
//            }
//            else if (i < 295)
//            {
//                _temperTable[i] = 5;
//                continue;
//            }
//            else if (i < 306)
//            {
//                _temperTable[i] = 6;
//                continue;
//            }
//            else if (i < 316)
//            {
//                _temperTable[i] = 7;
//                continue;
//            }
//            else if (i < 328)
//            {
//                _temperTable[i] = 8;
//                continue;
//            }
//            else if (i < 338)
//            {
//                _temperTable[i] = 9;
//                continue;
//            }
//            else if (i < 350)
//            {
//                _temperTable[i] = 10;
//                continue;
//            }
//            else if (i < 361)
//            {
//                _temperTable[i] = 11;
//                continue;
//            }
//            else if (i < 371)
//            {
//                _temperTable[i] = 12;
//                continue;
//            }
//            else if (i < 384)
//            {
//                _temperTable[i] = 13;
//                continue;
//            }
//            else if (i < 395)
//            {
//                _temperTable[i] = 14;
//                continue;
//            }
//            else if (i < 406)
//            {
//                _temperTable[i] = 15;
//                continue;
//            }
//            else if (i < 418)
//            {
//                _temperTable[i] = 16;
//                continue;
//            }
//            else if (i < 429)
//            {
//                _temperTable[i] = 17;
//                continue;
//            }
//            else if (i < 441)
//            {
//                _temperTable[i] = 18;
//                continue;
//            }
//            else if (i < 452)
//            {
//                _temperTable[i] = 19;
//                continue;
//            }
//            else if (i < 463)
//            {
//                _temperTable[i] = 20;
//                continue;
//            }
//            else if (i < 474)
//            {
//                _temperTable[i] = 21;
//                continue;
//            }
//            else if (i < 485)
//            {
//                _temperTable[i] = 22;
//                continue;
//            }
//            else if (i < 496)
//            {
//                _temperTable[i] = 23;
//                continue;
//            }
//            else if (i < 507)
//            {
//                _temperTable[i] = 24;
//                continue;
//            }
//            else if (i < 519)
//            {
//                _temperTable[i] = 25;
//                continue;
//            }
//            else if (i < 531)
//            {
//                _temperTable[i] = 26;
//                continue;
//            }
//            else if (i < 544)
//            {
//                _temperTable[i] = 27;
//                continue;
//            }
//            else if (i < 556)
//            {
//                _temperTable[i] = 28;
//                continue;
//            }
//            else if (i < 567)
//            {
//                _temperTable[i] = 29;
//                continue;
//            }
//            else if (i < 578)
//            {
//                _temperTable[i] = 30;
//                continue;
//            }
//            else if (i < 588)
//            {
//                _temperTable[i] = 31;
//                continue;
//            }
//            else if (i < 599)
//            {
//                _temperTable[i] = 32;
//                continue;
//            }
//            else if (i < 609)
//            {
//                _temperTable[i] = 33;
//                continue;
//            }
//            else if (i < 619)
//            {
//                _temperTable[i] = 34;
//                continue;
//            }
//            else if (i < 629)
//            {
//                _temperTable[i] = 35;
//                continue;
//            }
//            else if (i < 639)
//            {
//                _temperTable[i] = 36;
//                continue;
//            }
//            else if (i < 648)
//            {
//                _temperTable[i] = 37;
//                continue;
//            }
//            else if (i < 657)
//            {
//                _temperTable[i] = 38;
//                continue;
//            }
//            else if (i < 666)
//            {
//                _temperTable[i] = 39;
//                continue;
//            }
//            else if (i < 675)
//            {
//                _temperTable[i] = 40;
//                continue;
//            }
//            else if (i < 683)
//            {
//                _temperTable[i] = 41;
//                continue;
//            }
//            else if (i < 692)
//            {
//                _temperTable[i] = 42;
//                continue;
//            }
//            else if (i < 700)
//            {
//                _temperTable[i] = 43;
//                continue;
//            }
//            else if (i < 709)
//            {
//                _temperTable[i] = 44;
//                continue;
//            }
//            else if (i < 717)
//            {
//                _temperTable[i] = 45;
//                continue;
//            }
//            else if (i < 725)
//            {
//                _temperTable[i] = 46;
//                continue;
//            }
//            else if (i < 733)
//            {
//                _temperTable[i] = 47;
//                continue;
//            }
//            else if (i < 741)
//            {
//                _temperTable[i] = 48;
//                continue;
//            }
//            else if (i < 749)
//            {
//                _temperTable[i] = 49;
//                continue;
//            }
//            else if (i < 757)
//            {
//                _temperTable[i] = 50;
//                continue;
//            }
//            else if (i < 764)
//            {
//                _temperTable[i] = 51;
//                continue;
//            }
//            else if (i < 771)
//            {
//                _temperTable[i] = 52;
//                continue;
//            }
//            else if (i < 779)
//            {
//                _temperTable[i] = 53;
//                continue;
//            }
//            else if (i < 786)
//            {
//                _temperTable[i] = 54;
//                continue;
//            }
//            else if (i < 792)
//            {
//                _temperTable[i] = 55;
//                continue;
//            }
//            else if (i < 799)
//            {
//                _temperTable[i] = 56;
//                continue;
//            }
//            else if (i < 806)
//            {
//                _temperTable[i] = 57;
//                continue;
//            }
//            else if (i < 811)
//            {
//                _temperTable[i] = 58;
//                continue;
//            }
//            else if (i < 818)
//            {
//                _temperTable[i] = 59;
//                continue;
//            }
//            else if (i < 824)
//            {
//                _temperTable[i] = 60;
//                continue;
//            }
//            else if (i < 829)
//            {
//                _temperTable[i] = 61;
//                continue;
//            }
//            else if (i < 835)
//            {
//                _temperTable[i] = 62;
//                continue;
//            }
//            else if (i < 841)
//            {
//                _temperTable[i] = 63;
//                continue;
//            }
//            else if (i < 846)
//            {
//                _temperTable[i] = 64;
//                continue;
//            }
//            else if (i < 851)
//            {
//                _temperTable[i] = 65;
//                continue;
//            }
//            else if (i < 856)
//            {
//                _temperTable[i] = 66;
//                continue;
//            }
//            else if (i < 861)
//            {
//                _temperTable[i] = 67;
//                continue;
//            }
//            else if (i < 865)
//            {
//                _temperTable[i] = 68;
//                continue;
//            }
//            else if (i < 869)
//            {
//                _temperTable[i] = 69;
//                continue;
//            }
//            else if (i < 874)
//            {
//                _temperTable[i] = 70;
//                continue;
//            }
//            else if (i < 878)
//            {
//                _temperTable[i] = 71;
//                continue;
//            }
//            else if (i < 882)
//            {
//                _temperTable[i] = 72;
//                continue;
//            }
//            else if (i < 886)
//            {
//                _temperTable[i] = 73;
//                continue;
//            }
//            else if (i < 890)
//            {
//                _temperTable[i] = 74;
//                continue;
//            }
//            else if (i < 893)
//            {
//                _temperTable[i] = 75;
//                continue;
//            }
//            else if (i < 897)
//            {
//                _temperTable[i] = 76;
//                continue;
//            }
//            else if (i < 901)
//            {
//                _temperTable[i] = 77;
//                continue;
//            }
//            else if (i < 904)
//            {
//                _temperTable[i] = 78;
//                continue;
//            }
//            else if (i < 908)
//            {
//                _temperTable[i] = 79;
//                continue;
//            }
//            else if (i < 911)
//            {
//                _temperTable[i] = 80;
//                continue;
//            }
//            else if (i < 915)
//            {
//                _temperTable[i] = 81;
//                continue;
//            }
//            else if (i < 918)
//            {
//                _temperTable[i] = 82;
//                continue;
//            }
//            else if (i < 921)
//            {
//                _temperTable[i] = 83;
//                continue;
//            }
//            else if (i < 925)
//            {
//                _temperTable[i] = 84;
//                continue;
//            }
//            else if (i < 928)
//            {
//                _temperTable[i] = 85;
//                continue;
//            }
//            else if (i < 931)
//            {
//                _temperTable[i] = 86;
//                continue;
//            }
//            else if (i < 933)
//            {
//                _temperTable[i] = 87;
//                continue;
//            }
//            else if (i < 936)
//            {
//                _temperTable[i] = 88;
//                continue;
//            }
//            else if (i < 938)
//            {
//                _temperTable[i] = 89;
//                continue;
//            }
//            else if (i < 941)
//            {
//                _temperTable[i] = 90;
//                continue;
//            }
//            else if (i < 943)
//            {
//                _temperTable[i] = 91;
//                continue;
//            }
//            else if (i < 945)
//            {
//                _temperTable[i] = 92;
//                continue;
//            }
//            else if (i < 947)
//            {
//                _temperTable[i] = 93;
//                continue;
//            }
//            else if (i < 949)
//            {
//                _temperTable[i] = 94;
//                continue;
//            }
//            else if (i < 951)
//            {
//                _temperTable[i] = 95;
//                continue;
//            }
//            else if (i < 953)
//            {
//                _temperTable[i] = 96;
//                continue;
//            }
//            else if (i < 955)
//            {
//                _temperTable[i] = 97;
//                continue;
//            }
//            else if (i < 957)
//            {
//                _temperTable[i] = 98;
//                continue;
//            }
//            else if (i < 958)
//            {
//                _temperTable[i] = 99;
//                continue;
//            }
//            else if (i < 959)
//            {
//                _temperTable[i] = 100;
//                continue;
//            }
//        }
    }

private:
    int _temperTable[TABLE_SIZE];
};

#endif // SAMPLETOTEMPERATURE_H_

