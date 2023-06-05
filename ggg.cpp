#include <iostream>
#include <cstdarg>

using namespace std;

// Adam Tyton

class POLYNOMIAL {
public:

    int* arr;
    int rank;
    static int overloaded;

    POLYNOMIAL() {
        arr = new int[1];
        arr[0] = 0;
        rank = 0;
    }

    POLYNOMIAL(int rank, ...) {
        arr = new int[rank + 1];
        this->rank = rank;

        va_list vaList;
        va_start(vaList, rank);

        for (int i = 0; i <= rank; i++) {
            arr[i] = va_arg(vaList, int);
        }

        toPrime();
    }


    POLYNOMIAL(const POLYNOMIAL& o) {
        rank = o.rank;
//        overloaded = o.overloaded;

        arr = new int[rank + 1];

        for (int i = 0; i <= rank; ++i) {
            arr[i] = o.arr[i];
        }
    }

    ~POLYNOMIAL() {
        delete[] arr;
    }



    POLYNOMIAL& operator= (POLYNOMIAL pol) {
        delete[] arr;
        rank = pol.rank;

        arr = new int[rank + 1];

        for (int i = 0; i <= rank; ++i) {
            arr[i] = pol.arr[i];
        }

        return *this;
    }

    POLYNOMIAL operator- () {
        POLYNOMIAL pol = POLYNOMIAL(*this);
        for (int i = 0; i <= rank; ++i) {
            pol.arr[i] = -pol.arr[i];
        }
        return pol;
    }

//    POLYNOMIAL& operator+= (POLYNOMIAL& pol) {
//
//    }

    int nwd(int a, int b)
    {
        while (a != 0)
        {
            int tmp = a;
            a = b % a;
            b = tmp;
        }
        return b;

        //return (a == 0) ? b : nwd(b % a, a);
    }

    void toPrime() {
        if (rank == 0) {
            if (arr[0] != 0)
                arr[0] = (arr[0] < 0) ? -1 : 1;
            return;
        }

        int div = arr[0];
        int j = 1;

        for (int i = 1; i <= rank; ++i) {
            div = nwd(div, arr[j++]);
        }

        if (div < 0)
            div = -div;

        for (int i = 0; i <= rank; ++i) {
            arr[i] /= div;
        }


    }

    void resize(int newRank) {
        int* arrNew = new int[newRank + 1];
        for (int i = 0; i <= rank; i++) {
            arrNew[i] = arr[i];
        }

        for (int i = rank + 1; i <= newRank; ++i) {
            arrNew[i] = 0;
        }

        delete[] arr;

        arr = arrNew;

    }

    void cutTo(int newRank) {
        int dif = rank - newRank;
        int* arrNew = new int[newRank + 1];
        for (int i = 0; i <= newRank; i++) {
            arrNew[i] = arr[i + dif];
            //arrNew[i] = arr[i - (rank - newRank)];
        }

        rank = newRank;
        delete[] arr;

        arr = arrNew;

    }

    int checkRank() {
        int newRank = rank;
        for (int i = rank; i >= 0 ; i--) {
            if (arr[i] == 0)
                newRank--;
            else break;
        }
        return newRank;
    }

    void multScalar(int scalar) {
        for (int i = 0; i <= rank; ++i) {
            arr[i] *= scalar;
        }
    }

    POLYNOMIAL operator+= (POLYNOMIAL& pol) {
        int newRank = rank;
        if (rank != pol.rank) {
            newRank = (rank > pol.rank) ? rank : pol.rank;
            resize(newRank);
            pol.resize(newRank);
        }

        for (int i = 0; i <= newRank; ++i) {
            arr[i] += pol.arr[i];
        }

        rank = newRank;

        toPrime();
        int afterRank = checkRank();
        if (afterRank < newRank) {
            resize(afterRank);
        }

        rank = afterRank;

        pol.resize(pol.rank);

        return *this;
    }

    POLYNOMIAL operator-= (POLYNOMIAL& pol) {
        pol = -pol;
        *this += pol;
        pol = -pol;
        return *this;
    }

    POLYNOMIAL operator+ (POLYNOMIAL pol) {
        POLYNOMIAL newPol = POLYNOMIAL(*this);
        newPol += pol;
        return newPol;
    }

    POLYNOMIAL operator- (POLYNOMIAL pol) {
        POLYNOMIAL newPol = POLYNOMIAL(*this);
        newPol -= pol;
        return newPol;
    }

    POLYNOMIAL operator<<= (int move) {
        if (move > (rank + 1)) return *this;
        cutTo(rank - move);
        return *this;
    }

    POLYNOMIAL operator>>= (int move) {
        int newRank = rank + move;
        int* newArr = new int[newRank + 1];

        for (int i = 0; i < move; ++i) {
            newArr[i] = 0;
        }

        for (int i = move; i <= newRank; i++) {
            newArr[i] = arr[i - move];
        }

        delete[] arr;

        arr = newArr;
        rank = newRank;

        return *this;
    }

    POLYNOMIAL operator<< (int move) {
        POLYNOMIAL pol = POLYNOMIAL(*this);
        pol <<= move;
        return pol;
    }

    POLYNOMIAL operator>> (int move) {
        POLYNOMIAL pol = POLYNOMIAL(*this);
        pol >>= move;
        return pol;
    }


    POLYNOMIAL operator*= (POLYNOMIAL& pol) {

        int* newArr = new int[rank + pol.rank + 1];

        for (int i = 0; i <= rank; ++i) {
            for (int j = 0; j <= pol.rank; ++j) {
                newArr[i + j] = arr[i] * pol.arr[j];
            }
        }

        delete[] arr;
        arr = newArr;
        rank = rank + pol.rank;

        toPrime();

        return *this;
    }

    POLYNOMIAL operator/= (POLYNOMIAL pol) {
        if (rank < pol.rank) return *this;

        POLYNOMIAL cpPol = *this;

        int* newArr = new int[rank - pol.rank + 1];

        int j = rank - pol.rank;


        for (int i = rank; i >= pol.rank; i--) {
            pol >>= j;
            int div = cpPol.arr[i] /  pol.arr[i];
            newArr[j] = div;
            POLYNOMIAL p2 = pol;
            if (div != 0) {
                p2.multScalar(div);
                cpPol -= p2;
            }
            pol <<= j;
            j--;
        }

        delete[] arr;

        rank = rank - pol.rank;
        arr = newArr;


        return *this;
    }

    POLYNOMIAL operator%= (POLYNOMIAL& pol) {
        if (rank < pol.rank) return *this;

        POLYNOMIAL cpPol = *this;

        int j = rank - pol.rank;

        for (int i = rank; i >= pol.rank; i--) {
            pol >>= j;
            int div = cpPol.arr[i] /  pol.arr[i];
            POLYNOMIAL p2 = pol;
            if (div != 0) {
                p2.multScalar(div);
                cpPol -= p2;
            }
            pol <<= j;
            j--;
        }
        delete[] arr;

        rank = cpPol.checkRank();
        int* newArr = new int[rank + 1];

        for (int i = 0; i <= rank; ++i) {
            newArr[i] = cpPol.arr[i];
        }

        arr = newArr;

        return *this;
    }

    POLYNOMIAL operator* (POLYNOMIAL pol) {
        POLYNOMIAL cpPol = POLYNOMIAL(*this);
        cpPol *= pol;
        return cpPol;
    }

    POLYNOMIAL operator/ (POLYNOMIAL pol) {
        POLYNOMIAL cpPol = POLYNOMIAL(*this);
        cpPol /= pol;
        return cpPol;
    }

    POLYNOMIAL operator% (POLYNOMIAL pol) {
        POLYNOMIAL cpPol = POLYNOMIAL(*this);
        cpPol %= pol;
        return cpPol;
    }

    POLYNOMIAL& operator++ () {
        for (int i = 0; i <= rank; ++i) {
            arr[i] += 1;
        }
        return *this;
    }

    POLYNOMIAL operator++ (int) {
        POLYNOMIAL pol(*this);

        for (int i = 0; i <= rank; ++i) {
            pol.arr[i] += 1;
        }

        return pol;
    }

    POLYNOMIAL& operator-- () {
        for (int i = 0; i <= rank; ++i) {
            arr[i] -= 1;
        }
        return *this;
    }

    POLYNOMIAL operator-- (int) {
        POLYNOMIAL pol(*this);

        for (int i = 0; i <= rank; ++i) {
            pol.arr[i] -= 1;
        }

        return pol;
    }

//    void* operator new (size_t amount) {
//        return ::new char[amount];
//    }

//    void operator delete (void* p) {
//        ::delete (POLYNOMIAL*) p;
//    }


};

ostream& operator<< (ostream& stream, POLYNOMIAL pol) {
    stream << "( ";
    for (int i = 0; i < (pol.rank); i++) {
        stream << pol.arr[i] << ", ";
    }
    stream << pol.arr[pol.rank] << " )";
    return stream;
}

istream& operator>> (istream& stream, POLYNOMIAL& pol) {
    stream >> pol.rank;

    delete[] pol.arr;

    for (int i = 0; i <= pol.rank; ++i) {
        stream >> pol.arr[i];
    }
    return stream;
}

bool operator==(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    if (pol1.rank != pol2.rank)
        return false;

    bool isSame = true;
    for (int i = 0; i <= pol1.rank; ++i) {
        if (pol1.arr[i] != pol2.arr[i])
        {
            isSame = false;
            break;
        }
    }

    if (isSame)
        return true;

    POLYNOMIAL pol2N = -pol2;

    for (int i = 0; i <= pol2N.rank; ++i) {
        if (pol1.arr[i] != pol2N.arr[i])
            return false;
    }
    return true;
}

bool operator>(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    if (pol1.rank > pol2.rank)
        return true;
    if (pol1.rank < pol2.rank)
        return false;

    for (int i = pol1.rank; i > -1; --i) {
        if (pol1.arr[i] > pol2.arr[i])
            return true;
        if (pol1.arr[i] < pol2.arr[i])
            return false;
    }

    return false;
}

bool operator<(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    if (pol1.rank < pol2.rank)
        return true;
    if (pol1.rank > pol2.rank)
        return false;

    for (int i = pol1.rank; i > -1; --i) {
        if (pol1.arr[i] < pol2.arr[i])
            return true;
        else if (pol1.arr[i] < pol2.arr[i])
            return false;
    }

    return false;
}

bool operator<=(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    return !(pol1 > pol2);
}

bool operator>=(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    return !(pol1 < pol2);
}

bool operator!=(POLYNOMIAL& pol1, POLYNOMIAL& pol2) {
    return !(pol1 == pol2);
}


int main() {
cout << POLYNOMIAL(0,160) + POLYNOMIAL(0,135) << endl;
cout << POLYNOMIAL(5,-80, -138, 162, -78, -182, 135) + POLYNOMIAL(5,81, -93, 48, -182, -159, 181) << endl;
cout << POLYNOMIAL(3,-85, 191, -173, -110) + POLYNOMIAL(3,60, 21, -57, 79) << endl;
cout << POLYNOMIAL(7,125, 191, 85, 84, -84, 89, -162, 98) + POLYNOMIAL(7,-23, -44, 128, -56, -43, 163, 15, -186) << endl;
cout << POLYNOMIAL(7,156, -24, -7, -170, -164, 80, -34, -182) + POLYNOMIAL(7,142, -57, -30, -7, -59, -113, 153, 102) << endl;
cout << POLYNOMIAL(7,-156, -111, -30, 161, -91, 184, 189, 81) + POLYNOMIAL(7,-12, -5, -2, 191, -105, -195, -38, 96) << endl;
cout << POLYNOMIAL(4,6, 173, -55, -117, 198) + POLYNOMIAL(4,133, 19, -55, -5, -108) << endl;
cout << POLYNOMIAL(3,-161, 31, -159, 22) + POLYNOMIAL(3,-35, -1, -63, -23) << endl;
cout << POLYNOMIAL(1,-23, -60) + POLYNOMIAL(1,-14, -28) << endl;
cout << POLYNOMIAL(3,-55, -128, 98, 137) + POLYNOMIAL(3,6, -26, -100, 121) << endl;
cout << POLYNOMIAL(9,165, 81, -63, 113, -115, 35, 118, -103, 86, 8) + POLYNOMIAL(9,58, -74, 52, -9, -84, 96, -67, 70, -165, -123) << endl;
cout << POLYNOMIAL(6,30, 100, 76, -13, -167, -90, -117) + POLYNOMIAL(6,78, -100, 115, 138, 28, 188, 149) << endl;
cout << POLYNOMIAL(7,-35, 94, 34, 192, -162, 168, 8, -108) + POLYNOMIAL(7,63, -194, 67, -82, 137, 184, -98, -116) << endl;
cout << POLYNOMIAL(7,133, 156, 98, 114, 157, -120, 142, -153) + POLYNOMIAL(7,-2, -158, -126, -86, 55, -64, -11, 31) << endl;
cout << POLYNOMIAL(0,-120) + POLYNOMIAL(0,176) << endl;
cout << POLYNOMIAL(4,67, 83, 89, 143, -156) + POLYNOMIAL(4,47, 14, -153, 77, -58) << endl;
cout << POLYNOMIAL(3,77, 51, -43, 80) + POLYNOMIAL(3,-94, 173, 126, -81) << endl;
cout << POLYNOMIAL(8,-199, -86, -35, -130, 79, 148, 111, 3, -132) + POLYNOMIAL(8,106, -9, 186, -164, 142, 135, 103, -179, -59) << endl;
cout << POLYNOMIAL(0,8) + POLYNOMIAL(0,11) << endl;
cout << POLYNOMIAL(3,-183, -119, -50, 164) + POLYNOMIAL(3,179, 57, 120, -50) << endl;
cout << POLYNOMIAL(5,37, -105, 195, -63, 145, -36) + POLYNOMIAL(5,-137, -90, -161, 45, -97, 199) << endl;
cout << POLYNOMIAL(8,187, -191, -17, -169, -151, 70, -87, 114, 117) + POLYNOMIAL(8,-65, 2, 185, 61, 61, 165, 147, -161, -52) << endl;
cout << POLYNOMIAL(2,108, 180, 188) + POLYNOMIAL(2,77, 85, 160) << endl;
cout << POLYNOMIAL(3,-103, 67, 57, 140) + POLYNOMIAL(3,-76, 158, -1, 138) << endl;
cout << POLYNOMIAL(2,-114, -115, -59) + POLYNOMIAL(2,71, -19, 92) << endl;
cout << POLYNOMIAL(6,-82, -83, 47, -63, -145, 31, -43) + POLYNOMIAL(6,-116, 164, -16, 140, -188, -197, -145) << endl;
cout << POLYNOMIAL(2,-118, -193, 18) + POLYNOMIAL(2,-147, -77, -160) << endl;
cout << POLYNOMIAL(3,198, 21, -11, -75) + POLYNOMIAL(3,190, 100, 192, 140) << endl;
cout << POLYNOMIAL(8,-79, -82, -120, -105, -76, -63, -133, 84, -56) + POLYNOMIAL(8,189, 171, -62, 37, 53, -153, 116, 119, 187) << endl;
cout << POLYNOMIAL(9,48, 1, 161, -127, 77, -139, 100, -46, 87, 166) + POLYNOMIAL(9,-68, -54, -107, -97, 149, -138, 186, 145, 153, 164) << endl;
cout << POLYNOMIAL(9,-81, -95, -153, 199, -135, -107, 53, -11, 34, -116) + POLYNOMIAL(9,179, 199, -11, -65, -10, 177, -1, 189, -196, 163) << endl;
cout << POLYNOMIAL(1,-21, -165) + POLYNOMIAL(1,143, 65) << endl;
cout << POLYNOMIAL(2,137, -200, -10) + POLYNOMIAL(2,115, 62, 51) << endl;
cout << POLYNOMIAL(3,-54, -191, -168, -138) + POLYNOMIAL(3,-134, 28, -183, 56) << endl;
cout << POLYNOMIAL(5,74, 130, 161, -28, 77, -186) + POLYNOMIAL(5,-125, 84, -123, -168, 121, -28) << endl;
cout << POLYNOMIAL(1,158, 151) + POLYNOMIAL(1,42, -30) << endl;
cout << POLYNOMIAL(1,-98, 98) + POLYNOMIAL(1,-134, -67) << endl;
cout << POLYNOMIAL(3,1, -45, 85, 143) + POLYNOMIAL(3,-195, 176, -161, 100) << endl;
cout << POLYNOMIAL(7,71, -80, 7, -30, -46, -177, 43, 173) + POLYNOMIAL(7,99, -142, -170, 87, 173, -103, 57, -128) << endl;
cout << POLYNOMIAL(7,-96, -115, 186, -180, -3, 145, 104, 36) + POLYNOMIAL(7,-93, -51, -11, -20, 68, -158, -39, -52) << endl;
cout << POLYNOMIAL(2,-147, 1, 55) + POLYNOMIAL(2,170, -164, 116) << endl;
cout << POLYNOMIAL(0,106) + POLYNOMIAL(0,172) << endl;
cout << POLYNOMIAL(1,130, -12) + POLYNOMIAL(1,4, -28) << endl;
cout << POLYNOMIAL(3,108, 174, -156, 139) + POLYNOMIAL(3,123, -173, 17, -104) << endl;
cout << POLYNOMIAL(2,-102, 175, -22) + POLYNOMIAL(2,141, -171, 171) << endl;
cout << POLYNOMIAL(4,-125, -36, 149, 87, 168) + POLYNOMIAL(4,137, -178, 156, 38, 197) << endl;
cout << POLYNOMIAL(3,-47, -54, 17, 61) + POLYNOMIAL(3,-90, -56, 56, -87) << endl;
cout << POLYNOMIAL(4,-62, 16, 69, 200, 168) + POLYNOMIAL(4,54, -19, -197, 61, -47) << endl;
cout << POLYNOMIAL(1,-35, -85) + POLYNOMIAL(1,-94, 5) << endl;
cout << POLYNOMIAL(1,-194, 127) + POLYNOMIAL(1,-100, -98) << endl;
cout << POLYNOMIAL(6,-126, -181, 55, 81, -195, 82, 148) + POLYNOMIAL(6,117, -24, 164, -63, 45, -199, 103) << endl;
cout << POLYNOMIAL(6,44, 158, 181, 78, 163, -171, 177) + POLYNOMIAL(6,172, -104, 145, -172, -43, 80, -12) << endl;
cout << POLYNOMIAL(7,76, 97, -14, 140, 112, 64, 74, -75) + POLYNOMIAL(7,149, 71, -88, -159, -70, -25, -12, -37) << endl;
cout << POLYNOMIAL(8,-180, 112, -165, -177, -180, -104, 29, 107, 76) + POLYNOMIAL(8,-3, 23, -155, 149, 141, -21, 12, 126, -4) << endl;
cout << POLYNOMIAL(4,55, -35, -86, 189, 122) + POLYNOMIAL(4,84, -137, 85, 143, -200) << endl;
cout << POLYNOMIAL(0,-168) + POLYNOMIAL(0,17) << endl;
cout << POLYNOMIAL(9,-149, 161, -135, -39, -60, -117, 109, -77, 165, 120) + POLYNOMIAL(9,-28, 171, -125, 134, -114, -29, -116, -83, 188, 190) << endl;
cout << POLYNOMIAL(2,197, 174, -8) + POLYNOMIAL(2,-10, 68, -152) << endl;
cout << POLYNOMIAL(6,-158, 110, 0, -31, -141, 96, -151) + POLYNOMIAL(6,-86, -156, -39, -83, -175, -186, 42) << endl;
cout << POLYNOMIAL(8,-151, -169, -42, 69, -145, 97, 16, -118, -23) + POLYNOMIAL(8,152, 16, 93, -109, 22, 163, -132, 35, 88) << endl;
cout << POLYNOMIAL(9,-68, 149, 118, -30, -115, 180, -129, -88, 106, 9) + POLYNOMIAL(9,146, 20, -188, -181, -63, 176, -28, 198, 161, -57) << endl;
cout << POLYNOMIAL(0,-106) + POLYNOMIAL(0,-8) << endl;
cout << POLYNOMIAL(5,-154, -110, 45, 25, -89, -37) + POLYNOMIAL(5,100, -169, -105, -152, 69, -46) << endl;
cout << POLYNOMIAL(3,103, -77, 6, -23) + POLYNOMIAL(3,10, -118, -172, 121) << endl;
cout << POLYNOMIAL(0,113) + POLYNOMIAL(0,61) << endl;
cout << POLYNOMIAL(4,-154, -48, 53, 12, 118) + POLYNOMIAL(4,-43, -123, -123, 78, 194) << endl;
cout << POLYNOMIAL(5,-138, 90, -17, 108, 185, 147) + POLYNOMIAL(5,-104, -93, 169, -164, -50, -112) << endl;
cout << POLYNOMIAL(0,-68) + POLYNOMIAL(0,188) << endl;
cout << POLYNOMIAL(5,-73, 194, -70, -175, 94, 115) + POLYNOMIAL(5,143, 66, -145, -91, -179, 95) << endl;
cout << POLYNOMIAL(4,51, -70, -21, 32, 194) + POLYNOMIAL(4,-132, -23, -99, -184, -90) << endl;
cout << POLYNOMIAL(9,128, 13, -48, -166, 33, 14, -81, -114, 18, -112) + POLYNOMIAL(9,144, -2, -89, -158, -57, -36, -130, -37, -199, -55) << endl;
cout << POLYNOMIAL(9,-10, 61, -77, -181, -6, -142, 189, 126, 123, -148) + POLYNOMIAL(9,-95, 82, -70, 167, -163, -33, -9, -37, 158, 79) << endl;
cout << POLYNOMIAL(7,2, 139, -181, -34, -128, -193, -61, -128) + POLYNOMIAL(7,-59, -118, -160, -109, 143, 6, -51, -30) << endl;
cout << POLYNOMIAL(3,-55, 44, 180, -84) + POLYNOMIAL(3,29, -104, 106, 18) << endl;
cout << POLYNOMIAL(0,-19) + POLYNOMIAL(0,-104) << endl;
cout << POLYNOMIAL(3,14, -53, 68, 155) + POLYNOMIAL(3,124, -169, -91, -90) << endl;
cout << POLYNOMIAL(9,194, 131, 72, 23, 189, -24, -149, 15, -84, 21) + POLYNOMIAL(9,-5, -134, 133, 187, -123, 175, -193, 183, 23, -156) << endl;
cout << POLYNOMIAL(5,-136, 21, -77, 198, -84, 16) + POLYNOMIAL(5,13, -78, -176, 182, 89, -115) << endl;
cout << POLYNOMIAL(8,47, 68, -101, -175, 187, 197, -29, 13, -31) + POLYNOMIAL(8,166, -73, 21, 140, 100, -95, -192, 122, -131) << endl;
cout << POLYNOMIAL(2,-43, 7, 88) + POLYNOMIAL(2,28, 52, -113) << endl;
cout << POLYNOMIAL(5,5, 113, 185, 7, -124, 121) + POLYNOMIAL(5,72, 11, -98, 40, 46, 50) << endl;
cout << POLYNOMIAL(6,161, 28, 175, 3, 25, 147, 77) + POLYNOMIAL(6,-108, 85, -25, -50, -6, 141, -7) << endl;
cout << POLYNOMIAL(0,90) + POLYNOMIAL(0,124) << endl;
cout << POLYNOMIAL(2,-29, 3, -190) + POLYNOMIAL(2,-132, -16, -197) << endl;
cout << POLYNOMIAL(3,-136, 51, 143, 7) + POLYNOMIAL(3,8, 83, 79, -130) << endl;
cout << POLYNOMIAL(1,21, -137) + POLYNOMIAL(1,-118, -140) << endl;
cout << POLYNOMIAL(0,-167) + POLYNOMIAL(0,28) << endl;
cout << POLYNOMIAL(7,82, -87, -80, 172, 88, 67, -68, -91) + POLYNOMIAL(7,113, -164, 174, -95, -66, 45, -149, -78) << endl;
cout << POLYNOMIAL(3,20, -18, 54, -185) + POLYNOMIAL(3,138, -82, 155, -137) << endl;
cout << POLYNOMIAL(4,-10, -77, -64, 170, 127) + POLYNOMIAL(4,-135, -141, -167, -9, 189) << endl;
cout << POLYNOMIAL(4,-99, 100, 107, -189, 61) + POLYNOMIAL(4,-48, 87, -170, 77, -184) << endl;
cout << POLYNOMIAL(4,-151, -139, -98, 181, 173) + POLYNOMIAL(4,-32, -141, 100, 5, 72) << endl;
cout << POLYNOMIAL(6,-71, 50, -29, 38, 170, -80, 2) + POLYNOMIAL(6,-29, 24, -2, 117, -31, 33, 95) << endl;
cout << POLYNOMIAL(7,-43, -184, -125, 106, 60, 119, 11, 106) + POLYNOMIAL(7,148, -110, -22, -7, 200, 38, -193, 192) << endl;
cout << POLYNOMIAL(7,112, 170, 126, -41, -102, 181, 121, 26) + POLYNOMIAL(7,-178, -40, -84, 93, 86, -164, 0, -1) << endl;
cout << POLYNOMIAL(3,49, 79, 145, 10) + POLYNOMIAL(3,-14, -195, 103, 6) << endl;
cout << POLYNOMIAL(5,-75, -137, -144, -119, 176, -11) + POLYNOMIAL(5,-41, -36, -51, 173, 69, 78) << endl;
cout << POLYNOMIAL(0,38) + POLYNOMIAL(0,17) << endl;
cout << POLYNOMIAL(5,89, -195, 90, -79, -15, 66) + POLYNOMIAL(5,-100, 178, 101, 139, -66, -134) << endl;
cout << POLYNOMIAL(3,-96, 118, 55, 66) + POLYNOMIAL(3,-198, 69, -147, -103) << endl;
cout << POLYNOMIAL(8,-94, -5, -137, 48, -40, -180, 152, -88, 10) + POLYNOMIAL(8,-27, -10, -192, -19, 181, -5, 76, 166, 99) << endl;
cout << POLYNOMIAL(5,27, -72, 139, 193, -126, 62) + POLYNOMIAL(5,-191, 96, 27, 61, -119, 86) << endl;
cout << POLYNOMIAL(1,150, -93) + POLYNOMIAL(1,-166, -89) << endl;
cout << POLYNOMIAL(0,-29) + POLYNOMIAL(0,-188) << endl;
cout << POLYNOMIAL(2,-83, -161, 93) + POLYNOMIAL(2,-188, -175, -40) << endl;
cout << POLYNOMIAL(7,-122, -122, 158, -167, 142, -83, 66, -145) + POLYNOMIAL(7,-122, 47, -41, -107, -157, 165, -94, 161) << endl;
cout << POLYNOMIAL(5,123, 196, -38, 58, 1, 4) + POLYNOMIAL(5,188, -32, 114, -102, -89, -82) << endl;
cout << POLYNOMIAL(8,-125, 162, -10, -193, 15, -162, 11, 152, -197) + POLYNOMIAL(8,-165, -96, 134, 117, -103, 108, -198, 193, -159) << endl;
cout << POLYNOMIAL(6,-125, 134, -183, -90, -44, -164, -193) + POLYNOMIAL(6,-103, 49, 92, 3, 149, 44, -39) << endl;
cout << POLYNOMIAL(8,72, 132, 8, -116, 40, 56, 108, -113, -40) + POLYNOMIAL(8,8, -194, 179, 48, 132, -69, 89, 103, 11) << endl;
cout << POLYNOMIAL(7,-25, -77, -179, 165, 54, 39, 48, -53) + POLYNOMIAL(7,-98, 146, 58, -112, 18, 112, -98, 58) << endl;
cout << POLYNOMIAL(0,-182) + POLYNOMIAL(0,-39) << endl;
cout << POLYNOMIAL(4,-187, -137, -53, -79, 149) + POLYNOMIAL(4,-27, -134, 50, 3, -162) << endl;
cout << POLYNOMIAL(6,35, 28, 170, -155, 89, -67, -100) + POLYNOMIAL(6,-122, 179, -10, 102, -36, -179, 194) << endl;
cout << POLYNOMIAL(5,200, -97, 160, 108, -127, -186) + POLYNOMIAL(5,-164, -74, -66, -9, -37, -136) << endl;
cout << POLYNOMIAL(4,94, -67, -35, -136, 39) + POLYNOMIAL(4,-176, -31, 26, 179, -142) << endl;
cout << POLYNOMIAL(7,-157, -197, -30, 171, -171, -131, 137, 187) + POLYNOMIAL(7,-1, -20, -87, 120, -194, -167, 25, 194) << endl;
cout << POLYNOMIAL(9,-30, 157, -71, 119, 33, -175, 179, 162, -84, 35) + POLYNOMIAL(9,112, -198, 32, -186, 76, 25, -121, -92, -159, 14) << endl;
cout << POLYNOMIAL(3,-122, 186, 80, 99) + POLYNOMIAL(3,157, 186, -43, -165) << endl;
cout << POLYNOMIAL(9,-95, 8, -34, 127, 95, 112, -186, 38, 67, 144) + POLYNOMIAL(9,-49, -41, -167, -101, -45, -62, 163, 9, 89, -121) << endl;
cout << POLYNOMIAL(0,-95) + POLYNOMIAL(0,192) << endl;
cout << POLYNOMIAL(2,-5, -109, -174) + POLYNOMIAL(2,128, -121, -80) << endl;
cout << POLYNOMIAL(9,-49, 178, -84, 10, 112, -6, -45, 190, -22, 23) + POLYNOMIAL(9,9, 146, -15, 51, 96, 181, 25, -83, -88, -127) << endl;
cout << POLYNOMIAL(5,-134, 35, 194, 13, 13, 126) + POLYNOMIAL(5,32, 44, 79, 5, -187, 72) << endl;
cout << POLYNOMIAL(2,59, -76, -33) + POLYNOMIAL(2,-7, 99, 191) << endl;
cout << POLYNOMIAL(5,143, 75, 90, 174, -170, 14) + POLYNOMIAL(5,187, 68, -147, 30, 69, -173) << endl;
cout << POLYNOMIAL(5,128, 4, -169, 82, 68, -141) + POLYNOMIAL(5,75, 20, 123, 80, -2, 181) << endl;
cout << POLYNOMIAL(9,-102, 156, 195, 41, -198, 34, -184, 76, -185, 156) + POLYNOMIAL(9,51, 117, 187, 117, -90, -118, 182, -153, 100, 47) << endl;
cout << POLYNOMIAL(9,171, -183, -91, 29, 102, -142, 168, -30, 183, 113) + POLYNOMIAL(9,8, 153, 65, -112, 125, 71, -191, 32, 199, -189) << endl;
cout << POLYNOMIAL(7,39, -103, 106, -161, 23, 153, 26, 139) + POLYNOMIAL(7,-60, 115, 108, -70, -37, 4, 105, 158) << endl;
cout << POLYNOMIAL(5,-53, 160, -73, 154, 149, -62) + POLYNOMIAL(5,-62, -172, 39, 4, -26, -2) << endl;
cout << POLYNOMIAL(3,-88, 123, 16, 167) + POLYNOMIAL(3,-12, 43, -164, 0) << endl;
cout << POLYNOMIAL(2,-40, -41, -22) + POLYNOMIAL(2,-106, 96, 28) << endl;
cout << POLYNOMIAL(4,-114, 192, 86, 126, -22) + POLYNOMIAL(4,196, -34, -53, -56, 27) << endl;
cout << POLYNOMIAL(5,-199, -164, 23, -87, -58, 71) + POLYNOMIAL(5,17, -6, 145, -99, -166, 152) << endl;
cout << POLYNOMIAL(3,-92, -61, 77, -184) + POLYNOMIAL(3,-122, -93, 122, -190) << endl;
cout << POLYNOMIAL(4,-137, -176, 156, 4, -192) + POLYNOMIAL(4,-4, -43, 84, 80, -130) << endl;
cout << POLYNOMIAL(5,-78, 197, -173, -160, -147, -29) + POLYNOMIAL(5,-53, -70, 50, 102, 135, 87) << endl;
cout << POLYNOMIAL(0,21) + POLYNOMIAL(0,-49) << endl;
cout << POLYNOMIAL(1,-24, 150) + POLYNOMIAL(1,-61, 174) << endl;
cout << POLYNOMIAL(0,-56) + POLYNOMIAL(0,25) << endl;
cout << POLYNOMIAL(7,-101, -137, -53, 38, -101, 199, -128, 18) + POLYNOMIAL(7,-140, -184, -119, 124, 53, -195, -51, 173) << endl;
cout << POLYNOMIAL(9,-153, -93, 138, -190, -139, -61, 162, -80, 100, 110) + POLYNOMIAL(9,89, 0, -157, 142, -58, 194, 62, -73, -96, 78) << endl;
cout << POLYNOMIAL(8,190, 9, 138, 175, -79, 183, 44, -141, 140) + POLYNOMIAL(8,-64, 129, 197, -153, -63, 185, -183, 180, -46) << endl;
cout << POLYNOMIAL(1,93, 64) + POLYNOMIAL(1,-173, 19) << endl;
cout << POLYNOMIAL(2,94, 155, -106) + POLYNOMIAL(2,-63, 51, 149) << endl;
cout << POLYNOMIAL(6,-117, -13, 97, 12, 157, 140, 11) + POLYNOMIAL(6,128, -66, -109, 123, -102, -197, 23) << endl;
cout << POLYNOMIAL(7,-200, 126, 44, -119, -43, 150, -69, 79) + POLYNOMIAL(7,-173, -147, -172, -72, 78, -176, 162, -36) << endl;
cout << POLYNOMIAL(1,-59, 135) + POLYNOMIAL(1,-52, 14) << endl;
cout << POLYNOMIAL(4,126, -21, 181, 0, 64) + POLYNOMIAL(4,96, 174, 181, 97, -178) << endl;
cout << POLYNOMIAL(7,-136, 70, -40, 63, 115, -184, -111, 164) + POLYNOMIAL(7,-63, -45, -27, 147, 3, 56, -69, 178) << endl;
cout << POLYNOMIAL(4,51, 43, 77, -134, 126) + POLYNOMIAL(4,-96, -27, -11, 102, -63) << endl;
cout << POLYNOMIAL(0,27) + POLYNOMIAL(0,-96) << endl;
cout << POLYNOMIAL(1,-57, -9) + POLYNOMIAL(1,18, 11) << endl;
cout << POLYNOMIAL(0,-172) + POLYNOMIAL(0,-187) << endl;
cout << POLYNOMIAL(7,134, 9, 78, 44, -142, -41, 123, -174) + POLYNOMIAL(7,-139, -64, -100, 62, 32, 72, 88, 9) << endl;
cout << POLYNOMIAL(5,-17, 104, 167, 134, 28, -158) + POLYNOMIAL(5,-26, -55, 12, 58, 124, 84) << endl;
cout << POLYNOMIAL(2,-157, 87, 113) + POLYNOMIAL(2,-147, -119, -178) << endl;
cout << POLYNOMIAL(0,183) + POLYNOMIAL(0,19) << endl;
cout << POLYNOMIAL(5,192, -92, -181, 71, -59, 115) + POLYNOMIAL(5,35, 112, 107, -37, -165, -147) << endl;
cout << POLYNOMIAL(9,142, -149, -167, -47, -13, 140, -113, -58, -97, -174) + POLYNOMIAL(9,-108, -19, -110, 157, -166, -43, 196, 90, 195, 49) << endl;
cout << POLYNOMIAL(7,-5, -190, 183, -99, 99, -88, 135, -123) + POLYNOMIAL(7,133, 26, -140, -59, 182, -30, 188, -122) << endl;
cout << POLYNOMIAL(5,116, -176, 69, -13, 184, -136) + POLYNOMIAL(5,-40, 153, -71, -168, -27, 128) << endl;
cout << POLYNOMIAL(6,-151, 147, 158, 71, -12, 69, -108) + POLYNOMIAL(6,-72, 189, -25, 176, -191, -197, 77) << endl;
cout << POLYNOMIAL(2,-125, 1, 34) + POLYNOMIAL(2,-162, -9, 23) << endl;
cout << POLYNOMIAL(8,-195, -121, 72, -51, 40, 87, -136, 61, -134) + POLYNOMIAL(8,-138, -91, -159, 85, -58, -145, -12, 5, -124) << endl;
cout << POLYNOMIAL(4,132, -88, 116, 88, -200) + POLYNOMIAL(4,-116, -156, -32, 96, 45) << endl;
cout << POLYNOMIAL(6,62, 4, -22, -182, -76, 34, -28) + POLYNOMIAL(6,16, 99, 15, 58, 159, -135, 29) << endl;
cout << POLYNOMIAL(5,133, 180, 148, 77, -186, -106) + POLYNOMIAL(5,136, -119, -61, 35, 167, 51) << endl;
cout << POLYNOMIAL(8,199, 104, 49, -19, -70, -178, 70, 176, 57) + POLYNOMIAL(8,-101, -96, 5, -10, 85, 119, 115, -68, -29) << endl;
cout << POLYNOMIAL(5,116, -144, 9, -58, 4, 68) + POLYNOMIAL(5,178, 115, 18, 10, 153, 155) << endl;
cout << POLYNOMIAL(3,61, -166, 126, -44) + POLYNOMIAL(3,96, 146, 5, -9) << endl;
cout << POLYNOMIAL(4,-85, 106, -152, 53, 175) + POLYNOMIAL(4,-134, 157, -45, -55, -101) << endl;
cout << POLYNOMIAL(6,49, 124, 185, -99, -60, 176, 4) + POLYNOMIAL(6,187, 57, -9, -10, -158, 62, -78) << endl;
cout << POLYNOMIAL(6,90, 117, 77, 181, 153, -135, 147) + POLYNOMIAL(6,-153, -184, 121, 117, -138, -11, -60) << endl;
cout << POLYNOMIAL(4,198, 139, 54, 138, 25) + POLYNOMIAL(4,96, 105, -185, -29, -180) << endl;
cout << POLYNOMIAL(5,-112, 138, -122, 85, 30, 151) + POLYNOMIAL(5,81, 158, 11, -142, 97, 198) << endl;
cout << POLYNOMIAL(5,52, 49, -128, 199, -142, -109) + POLYNOMIAL(5,-150, -88, 139, -89, -54, -31) << endl;
cout << POLYNOMIAL(6,45, -74, 103, -92, 28, 18, 54) + POLYNOMIAL(6,195, 23, -7, 162, 118, -19, 134) << endl;
cout << POLYNOMIAL(0,41) + POLYNOMIAL(0,-65) << endl;
cout << POLYNOMIAL(3,97, -128, -63, -32) + POLYNOMIAL(3,-192, -36, -82, -17) << endl;
cout << POLYNOMIAL(0,126) + POLYNOMIAL(0,198) << endl;
cout << POLYNOMIAL(3,133, -182, 56, 24) + POLYNOMIAL(3,-139, -13, 4, -58) << endl;
cout << POLYNOMIAL(0,113) + POLYNOMIAL(0,-195) << endl;
cout << POLYNOMIAL(9,128, 20, 48, -115, 152, -64, -157, 188, -152, -114) + POLYNOMIAL(9,-52, -141, 77, 72, -48, 136, 7, 184, 83, 17) << endl;
cout << POLYNOMIAL(0,60) + POLYNOMIAL(0,129) << endl;
cout << POLYNOMIAL(9,1, 62, -108, 172, -81, 112, -160, 44, 198, -173) + POLYNOMIAL(9,58, 27, -152, -142, 182, -127, -22, -109, 30, 176) << endl;
cout << POLYNOMIAL(4,113, -39, -123, 19, 36) + POLYNOMIAL(4,16, -47, -193, 186, 137) << endl;
cout << POLYNOMIAL(4,15, -35, -44, 65, -65) + POLYNOMIAL(4,-4, 74, -123, -105, 74) << endl;
cout << POLYNOMIAL(1,196, 95) + POLYNOMIAL(1,25, 51) << endl;
cout << POLYNOMIAL(4,83, -94, -101, 190, -124) + POLYNOMIAL(4,-117, 83, 66, 99, -7) << endl;
cout << POLYNOMIAL(6,123, -69, 62, -146, -9, -34, 56) + POLYNOMIAL(6,-82, -3, -139, -134, -200, 164, 24) << endl;
cout << POLYNOMIAL(9,83, -164, 93, 35, 63, -34, -125, 29, 125, 161) + POLYNOMIAL(9,114, 194, -120, 152, 147, 62, 138, 106, 120, -159) << endl;
cout << POLYNOMIAL(4,-22, -146, -128, -116, -195) + POLYNOMIAL(4,93, 34, 166, -104, -188) << endl;
cout << POLYNOMIAL(2,81, 91, -99) + POLYNOMIAL(2,-91, -164, -8) << endl;
cout << POLYNOMIAL(2,-48, 4, -45) + POLYNOMIAL(2,95, -51, -56) << endl;
cout << POLYNOMIAL(1,-93, -134) + POLYNOMIAL(1,-81, 38) << endl;
cout << POLYNOMIAL(3,92, -76, -132, -146) + POLYNOMIAL(3,134, 138, -95, 167) << endl;
cout << POLYNOMIAL(4,-170, 93, -170, -182, -119) + POLYNOMIAL(4,43, -195, 99, 86, 185) << endl;
cout << POLYNOMIAL(2,-84, 0, -128) + POLYNOMIAL(2,45, 166, -199) << endl;
cout << POLYNOMIAL(5,-195, 41, -78, 98, 148, 184) + POLYNOMIAL(5,60, 6, -62, -37, 169, -129) << endl;
cout << POLYNOMIAL(5,184, -98, -109, 156, -70, 114) + POLYNOMIAL(5,-38, -187, -184, 98, 13, -118) << endl;
cout << POLYNOMIAL(7,85, -138, -40, -41, 127, 135, -38, 150) + POLYNOMIAL(7,-33, 197, -14, 112, 15, 80, -44, 189) << endl;
cout << POLYNOMIAL(3,-140, 160, -124, -130) + POLYNOMIAL(3,-94, 87, 154, 126) << endl;
cout << POLYNOMIAL(8,76, 189, -8, 136, -88, 182, -59, -142, 71) + POLYNOMIAL(8,-68, 29, 165, -72, -199, -115, 13, -177, -98) << endl;
cout << POLYNOMIAL(2,122, 57, -141) + POLYNOMIAL(2,-67, 54, -118) << endl;
cout << POLYNOMIAL(8,165, 193, -164, 86, -169, 127, 88, 154, -126) + POLYNOMIAL(8,87, -156, -174, 148, 180, -9, -131, -142, -116) << endl;
cout << POLYNOMIAL(1,-184, -157) + POLYNOMIAL(1,-115, 169) << endl;
cout << POLYNOMIAL(6,-75, -186, 171, 116, 50, 162, 91) + POLYNOMIAL(6,-88, 32, 161, 155, 74, -14, -170) << endl;
cout << POLYNOMIAL(4,197, 65, -142, -136, 146) + POLYNOMIAL(4,-67, -145, 170, 82, 130) << endl;
cout << POLYNOMIAL(3,-33, 22, 117, 36) + POLYNOMIAL(3,-143, 81, -116, -31) << endl;
cout << POLYNOMIAL(0,-45) + POLYNOMIAL(0,-55) << endl;
cout << POLYNOMIAL(3,-170, -146, -7, 8) + POLYNOMIAL(3,38, -72, 65, -181) << endl;
cout << POLYNOMIAL(3,-178, -58, -158, -91) + POLYNOMIAL(3,-49, 154, 101, 180) << endl;
cout << POLYNOMIAL(0,186) + POLYNOMIAL(0,-162) << endl;
cout << POLYNOMIAL(7,-166, 55, 13, 200, -164, -18, 38, -32) + POLYNOMIAL(7,-90, 128, -80, -182, -171, 151, -127, -118) << endl;
cout << POLYNOMIAL(7,143, -28, -157, 12, -154, -191, 155, -148) + POLYNOMIAL(7,-85, -174, -4, 64, -26, 32, 74, -34) << endl;
cout << POLYNOMIAL(6,-95, 135, -147, 60, 160, 200, -103) + POLYNOMIAL(6,-113, -102, 59, 167, -182, -34, -98) << endl;
cout << POLYNOMIAL(6,-149, 190, 189, -80, 137, -129, -154) + POLYNOMIAL(6,17, -51, 27, 165, 66, -154, 85) << endl;
cout << POLYNOMIAL(2,-5, 165, 28) + POLYNOMIAL(2,42, 141, -193) << endl;
cout << POLYNOMIAL(7,-169, 115, -134, 54, -57, -173, 87, -63) + POLYNOMIAL(7,-144, 16, -128, 149, -148, 191, -153, 20) << endl;
cout << POLYNOMIAL(2,-76, -22, 24) + POLYNOMIAL(2,-50, 56, 80) << endl;
cout << POLYNOMIAL(6,-190, -152, -105, 169, 66, -44, 165) + POLYNOMIAL(6,172, 61, -192, -53, 123, -148, 114) << endl;
cout << POLYNOMIAL(9,192, -59, 156, -179, -151, 197, 35, 13, 29, -191) + POLYNOMIAL(9,104, -110, 75, 186, -199, -164, 171, -169, -108, -136) << endl;
cout << POLYNOMIAL(9,58, -28, 154, 20, 27, -168, -137, 22, -54, -70) + POLYNOMIAL(9,-123, 158, 65, -29, 36, 10, 143, 172, 170, 54) << endl;
cout << POLYNOMIAL(1,-17, -101) + POLYNOMIAL(1,193, 108) << endl;
cout << POLYNOMIAL(2,45, -78, -193) + POLYNOMIAL(2,51, 22, 76) << endl;
cout << POLYNOMIAL(1,51, 32) + POLYNOMIAL(1,3, 131) << endl;
cout << POLYNOMIAL(8,-196, 156, 7, 103, -72, 157, -62, -8, -76) + POLYNOMIAL(8,-153, 33, 140, 61, 89, -186, 59, 136, -107) << endl;
cout << POLYNOMIAL(8,-126, 168, 98, 182, -78, -145, 76, 190, 156) + POLYNOMIAL(8,195, -84, -83, 68, -52, -128, -12, -159, 12) << endl;
cout << POLYNOMIAL(1,95, -154) + POLYNOMIAL(1,-131, 104) << endl;
cout << POLYNOMIAL(8,58, 3, -8, 150, -30, -62, -101, 195, -46) + POLYNOMIAL(8,-190, 116, -31, 60, -198, -143, 19, 65, 143) << endl;
cout << POLYNOMIAL(1,-145, 152) + POLYNOMIAL(1,-177, 5) << endl;
cout << POLYNOMIAL(2,18, -77, 17) + POLYNOMIAL(2,72, -194, 94) << endl;
cout << POLYNOMIAL(7,102, -12, -17, 44, 8, -34, -12, -15) + POLYNOMIAL(7,11, 147, -124, -195, -197, 78, 31, 195) << endl;
cout << POLYNOMIAL(2,-23, -101, -52) + POLYNOMIAL(2,-190, -91, -53) << endl;
cout << POLYNOMIAL(9,-99, -39, 136, -131, 161, 29, 14, -183, -12, 73) + POLYNOMIAL(9,-100, -97, 101, 70, 38, -111, -135, -94, -190, -150) << endl;
cout << POLYNOMIAL(9,-198, 13, 33, -51, 102, 127, -104, -114, 112, -78) + POLYNOMIAL(9,196, 146, 134, 177, -102, -110, -1, 51, -162, 33) << endl;
cout << POLYNOMIAL(1,-29, 175) + POLYNOMIAL(1,-85, 80) << endl;
cout << POLYNOMIAL(4,-2, 119, 99, 147, 141) + POLYNOMIAL(4,1, 165, -5, -100, -143) << endl;
cout << POLYNOMIAL(0,-1) + POLYNOMIAL(0,21) << endl;
cout << POLYNOMIAL(3,119, -88, 106, -191) + POLYNOMIAL(3,107, -126, -155, -73) << endl;
cout << POLYNOMIAL(3,121, -186, -2, 74) + POLYNOMIAL(3,165, -182, 77, -160) << endl;
cout << POLYNOMIAL(3,-79, 178, 112, -81) + POLYNOMIAL(3,-157, 165, -67, -15) << endl;
cout << POLYNOMIAL(9,2, 41, -30, 33, -135, -164, -52, 162, 85, 170) + POLYNOMIAL(9,-161, 76, 115, 35, 32, -33, -94, 84, 0, -131) << endl;
cout << POLYNOMIAL(5,181, -167, 121, 66, -59, -12) + POLYNOMIAL(5,-179, -162, -72, -84, -132, -176) << endl;
cout << POLYNOMIAL(4,109, 11, 175, -112, 69) + POLYNOMIAL(4,-19, -110, 149, 136, 1) << endl;
cout << POLYNOMIAL(6,-84, -34, -156, -120, 128, -60, 23) + POLYNOMIAL(6,-181, 46, 108, 81, -84, -200, -190) << endl;
cout << POLYNOMIAL(5,111, 79, -152, -112, -100, 111) + POLYNOMIAL(5,-141, -161, 185, -147, -59, 21) << endl;
cout << POLYNOMIAL(3,187, -49, 198, 118) + POLYNOMIAL(3,-58, 112, 84, 96) << endl;
cout << POLYNOMIAL(4,-178, -107, 28, -37, -40) + POLYNOMIAL(4,153, -32, -72, 188, -24) << endl;
cout << POLYNOMIAL(7,180, 129, -29, 147, -14, 92, 126, -48) + POLYNOMIAL(7,-158, 21, -77, -168, 52, -34, -37, -126) << endl;
cout << POLYNOMIAL(2,-114, -6, 133) + POLYNOMIAL(2,-11, 195, 49) << endl;
cout << POLYNOMIAL(4,-134, 87, -97, -168, -47) + POLYNOMIAL(4,-59, -42, 10, -190, 13) << endl;
cout << POLYNOMIAL(0,-187) + POLYNOMIAL(0,-39) << endl;
cout << POLYNOMIAL(5,-2, 54, -163, -148, 132, -177) + POLYNOMIAL(5,132, -41, -146, -199, -96, 116) << endl;
cout << POLYNOMIAL(5,7, 167, 93, 177, 152, 86) + POLYNOMIAL(5,138, -115, 134, 159, -71, -160) << endl;
cout << POLYNOMIAL(4,68, 184, 23, 199, -95) + POLYNOMIAL(4,35, -194, 1, 2, 22) << endl;
cout << POLYNOMIAL(4,-188, 60, 135, 117, 160) + POLYNOMIAL(4,30, -82, 105, 3, 129) << endl;
cout << POLYNOMIAL(4,158, -125, 8, -140, 30) + POLYNOMIAL(4,-78, -83, 17, -8, 128) << endl;
cout << POLYNOMIAL(3,153, 167, 158, -53) + POLYNOMIAL(3,146, -91, 63, -143) << endl;
cout << POLYNOMIAL(7,-57, 32, -44, -196, 46, 4, 127, -61) + POLYNOMIAL(7,-42, 149, 112, 130, 185, 98, 22, -96) << endl;
cout << POLYNOMIAL(1,-165, -142) + POLYNOMIAL(1,52, -168) << endl;
cout << POLYNOMIAL(4,-97, -39, -123, -128, 60) + POLYNOMIAL(4,-175, -119, 53, -114, -13) << endl;
cout << POLYNOMIAL(1,-28, -50) + POLYNOMIAL(1,-114, -58) << endl;
cout << POLYNOMIAL(2,59, 146, -15) + POLYNOMIAL(2,-197, 3, 89) << endl;
cout << POLYNOMIAL(5,-80, 127, -4, -199, -120, 87) + POLYNOMIAL(5,26, 126, 144, 163, -117, -70) << endl;
cout << POLYNOMIAL(9,-185, 19, -91, 60, -175, 21, 18, 99, -123, 159) + POLYNOMIAL(9,187, 164, -179, 121, -94, 72, -90, 129, -178, -139) << endl;
cout << POLYNOMIAL(1,-110, 194) + POLYNOMIAL(1,-37, 41) << endl;
cout << POLYNOMIAL(5,-81, 5, -149, 162, 0, -2) + POLYNOMIAL(5,-51, 72, -136, -74, 39, -94) << endl;
cout << POLYNOMIAL(2,-23, 153, -51) + POLYNOMIAL(2,10, -114, -27) << endl;
cout << POLYNOMIAL(5,198, 19, -94, 143, -96, -37) + POLYNOMIAL(5,126, 105, -161, -107, -165, -130) << endl;
cout << POLYNOMIAL(2,-108, 105, -42) + POLYNOMIAL(2,60, 59, 29) << endl;
cout << POLYNOMIAL(8,157, -59, 64, 58, -122, 116, 132, 58, -127) + POLYNOMIAL(8,77, -136, 23, -85, 132, 186, 1, -4, 127) << endl;
cout << POLYNOMIAL(3,-91, 173, -76, -157) + POLYNOMIAL(3,97, -122, -102, -108) << endl;
cout << POLYNOMIAL(2,37, 18, 144) + POLYNOMIAL(2,28, 162, -169) << endl;
cout << POLYNOMIAL(1,39, 1) + POLYNOMIAL(1,-97, 66) << endl;
cout << POLYNOMIAL(0,-17) + POLYNOMIAL(0,172) << endl;
cout << POLYNOMIAL(7,-150, -20, 155, 62, 8, 69, -182, 28) + POLYNOMIAL(7,24, 17, -100, -75, -65, -89, 66, -14) << endl;
cout << POLYNOMIAL(0,-38) + POLYNOMIAL(0,62) << endl;
cout << POLYNOMIAL(5,10, -195, -39, 16, 43, 51) + POLYNOMIAL(5,-86, 63, -190, 124, 2, 66) << endl;
cout << POLYNOMIAL(7,-155, 155, -150, -5, 45, -115, -25, -119) + POLYNOMIAL(7,-3, 200, -101, -200, 6, -138, 143, -154) << endl;
cout << POLYNOMIAL(7,176, 2, 124, 125, 106, 152, -165, -103) + POLYNOMIAL(7,-105, -106, 183, -13, 92, 171, -75, -2) << endl;
cout << POLYNOMIAL(1,-12, 35) + POLYNOMIAL(1,-121, 11) << endl;
cout << POLYNOMIAL(7,114, 2, -73, -1, 39, 153, -190, 5) + POLYNOMIAL(7,125, 114, -146, 131, -34, 84, 103, 142) << endl;
cout << POLYNOMIAL(8,-106, -145, -120, 101, 85, -118, -140, -112, -154) + POLYNOMIAL(8,186, -111, 69, 148, -181, -74, -115, 32, -162) << endl;
cout << POLYNOMIAL(7,166, -160, -54, -156, 4, -59, 148, 69) + POLYNOMIAL(7,-149, -150, -172, -106, -15, 52, -193, -162) << endl;
cout << POLYNOMIAL(4,71, 42, 116, 176, 99) + POLYNOMIAL(4,170, 85, -83, 132, -151) << endl;
cout << POLYNOMIAL(1,-67, -101) + POLYNOMIAL(1,151, 43) << endl;
cout << POLYNOMIAL(6,151, 33, 42, 30, 151, 169, 2) + POLYNOMIAL(6,134, -191, -58, -64, 23, 2, -93) << endl;
cout << POLYNOMIAL(1,-110, -69) + POLYNOMIAL(1,-7, 154) << endl;
cout << POLYNOMIAL(5,91, 124, -21, 176, -179, -127) + POLYNOMIAL(5,-40, 90, -91, -174, -51, 28) << endl;
cout << POLYNOMIAL(7,-13, -40, 79, 23, -196, -30, -176, 165) + POLYNOMIAL(7,78, 105, -145, 183, 161, -6, -133, -4) << endl;
cout << POLYNOMIAL(4,-174, -152, -127, 132, 14) + POLYNOMIAL(4,194, 35, 85, -114, 78) << endl;
cout << POLYNOMIAL(8,-5, 43, -145, 107, 152, 169, 126, 169, -81) + POLYNOMIAL(8,-168, -45, 107, -106, -182, -136, 184, 50, 105) << endl;
cout << POLYNOMIAL(8,-91, 128, -48, 138, 177, -23, 49, -178, -94) + POLYNOMIAL(8,-127, 29, -3, -82, 30, 156, -192, -114, -126) << endl;
cout << POLYNOMIAL(3,18, 94, 92, 162) + POLYNOMIAL(3,-186, -75, -54, 91) << endl;
cout << POLYNOMIAL(9,-156, 57, -33, -128, -2, 52, 85, 198, -102, 6) + POLYNOMIAL(9,51, -141, 37, 169, 85, 113, 89, 46, 92, 108) << endl;
cout << POLYNOMIAL(0,-81) + POLYNOMIAL(0,-79) << endl;
cout << POLYNOMIAL(1,138, 57) + POLYNOMIAL(1,123, -65) << endl;
cout << POLYNOMIAL(9,-26, -1, -123, -153, -167, 189, 87, 0, -199, 12) + POLYNOMIAL(9,-67, -90, -145, 0, -102, -111, -157, 163, 25, 31) << endl;
cout << POLYNOMIAL(9,112, -65, -196, -82, -144, 82, -89, -26, 44, 192) + POLYNOMIAL(9,-17, -157, 127, -31, -95, -136, 50, -54, 31, -129) << endl;
cout << POLYNOMIAL(5,82, 79, -165, 180, 150, -95) + POLYNOMIAL(5,-149, 124, 70, -160, -132, -134) << endl;
cout << POLYNOMIAL(8,171, 132, 21, -25, -63, -138, -149, 116, 129) + POLYNOMIAL(8,194, 137, -33, -42, -96, 53, 16, 174, -74) << endl;
cout << POLYNOMIAL(4,-45, 75, -158, -167, -136) + POLYNOMIAL(4,128, 188, 108, 183, 187) << endl;
cout << POLYNOMIAL(5,-60, 41, 193, -121, 21, -162) + POLYNOMIAL(5,-17, -186, -44, -175, 17, 199) << endl;
cout << POLYNOMIAL(5,-127, -152, -34, 115, -51, 15) + POLYNOMIAL(5,-56, -39, -162, 128, -140, -94) << endl;
cout << POLYNOMIAL(9,144, 94, 96, -45, 5, 119, 144, 1, -56, 191) + POLYNOMIAL(9,105, 129, -195, -45, -13, -95, 65, -167, -72, 164) << endl;
cout << POLYNOMIAL(1,142, -77) + POLYNOMIAL(1,161, -99) << endl;
cout << POLYNOMIAL(2,46, 21, -126) + POLYNOMIAL(2,145, 182, -73) << endl;
cout << POLYNOMIAL(5,2, 67, -197, 147, -118, -149) + POLYNOMIAL(5,-64, 94, 3, 99, 57, 39) << endl;
cout << POLYNOMIAL(0,-177) + POLYNOMIAL(0,83) << endl;
cout << POLYNOMIAL(5,-75, -90, -99, 53, -152, -39) + POLYNOMIAL(5,-112, 147, 193, -69, 167, 168) << endl;
cout << POLYNOMIAL(4,131, -9, -33, 184, -97) + POLYNOMIAL(4,156, 171, 129, 9, 65) << endl;
cout << POLYNOMIAL(8,129, -155, -100, 7, -126, -153, -42, 8, 19) + POLYNOMIAL(8,-43, -43, -77, 196, -191, -2, -60, 188, -68) << endl;
cout << POLYNOMIAL(2,32, -189, 54) + POLYNOMIAL(2,-180, 34, 127) << endl;
cout << POLYNOMIAL(7,181, 37, -132, 180, -86, -44, -140, 28) + POLYNOMIAL(7,61, -43, 24, 181, -185, 101, -30, -116) << endl;
cout << POLYNOMIAL(1,-200, 130) + POLYNOMIAL(1,-84, -130) << endl;
cout << POLYNOMIAL(4,74, -90, -84, -95, -2) + POLYNOMIAL(4,70, -51, -151, -106, -3) << endl;
cout << POLYNOMIAL(9,8, 31, 6, 155, 185, 44, -180, 190, 178, -31) + POLYNOMIAL(9,8, -126, 198, 20, 91, -113, -192, -164, -147, -3) << endl;
cout << POLYNOMIAL(2,-163, 78, 25) + POLYNOMIAL(2,118, 162, 80) << endl;
cout << POLYNOMIAL(3,-139, 118, -55, -57) + POLYNOMIAL(3,31, 192, -43, -159) << endl;
cout << POLYNOMIAL(6,-81, -82, -136, -95, -167, 77, 58) + POLYNOMIAL(6,26, 107, 194, 5, -10, 172, -77) << endl;
cout << POLYNOMIAL(4,68, -81, -79, 9, 96) + POLYNOMIAL(4,-62, 87, -19, 98, 62) << endl;
cout << POLYNOMIAL(9,199, -20, -98, 78, -47, -163, -132, -6, 136, 113) + POLYNOMIAL(9,-168, 17, 186, -100, 190, -38, 47, 187, -174, 59) << endl;
cout << POLYNOMIAL(6,-159, 58, -6, 78, 120, 92, 113) + POLYNOMIAL(6,-116, -190, -21, -159, 29, 72, -148) << endl;
cout << POLYNOMIAL(1,13, -133) + POLYNOMIAL(1,-22, 36) << endl;
cout << POLYNOMIAL(5,-168, 60, 137, -86, 31, 99) + POLYNOMIAL(5,64, -157, 80, -3, -164, 119) << endl;
cout << POLYNOMIAL(7,-23, 7, -174, -160, 165, 161, 194, 35) + POLYNOMIAL(7,46, 131, -134, 154, 45, 54, 104, 34) << endl;
cout << POLYNOMIAL(0,-21) + POLYNOMIAL(0,-65) << endl;
cout << POLYNOMIAL(8,7, -54, 90, -67, 102, 118, 121, -39, -182) + POLYNOMIAL(8,-10, 144, -151, 199, -82, -29, 175, -75, -134) << endl;
cout << POLYNOMIAL(5,108, -117, 28, 119, -3, -167) + POLYNOMIAL(5,-19, 17, 12, 127, 69, 15) << endl;
cout << POLYNOMIAL(7,-129, -187, 103, 4, 76, -27, -142, 73) + POLYNOMIAL(7,185, -80, 31, -139, -146, 42, 74, 28) << endl;
cout << POLYNOMIAL(6,-91, 128, -28, 198, 155, 50, 93) + POLYNOMIAL(6,98, -87, -5, 174, -176, 125, 56) << endl;
cout << POLYNOMIAL(2,108, -164, 70) + POLYNOMIAL(2,-38, 8, 72) << endl;
cout << POLYNOMIAL(1,126, 87) + POLYNOMIAL(1,6, -37) << endl;
cout << POLYNOMIAL(4,-197, -174, 117, 75, 159) + POLYNOMIAL(4,-151, -124, -180, -96, 85) << endl;
cout << POLYNOMIAL(7,167, 183, 48, 179, 185, 160, -136, 193) + POLYNOMIAL(7,24, 67, -151, 140, -100, 17, 117, 136) << endl;
cout << POLYNOMIAL(8,-7, 29, -86, -7, -124, -15, -172, -188, -177) + POLYNOMIAL(8,76, -115, -135, 76, -177, -148, 27, 138, 159) << endl;
cout << POLYNOMIAL(8,63, -95, 94, -171, 73, 150, -62, 119, -178) + POLYNOMIAL(8,32, 24, 123, -30, 138, 189, -73, 138, -1) << endl;
cout << POLYNOMIAL(6,141, -75, 131, -129, 53, -128, 155) + POLYNOMIAL(6,158, -163, 99, -163, 144, 96, 136) << endl;
cout << POLYNOMIAL(7,119, 21, 98, -99, -149, -83, -131, -71) + POLYNOMIAL(7,-129, 77, -90, -185, -108, -118, -194, -64) << endl;
cout << POLYNOMIAL(1,-36, -81) + POLYNOMIAL(1,-51, -32) << endl;
cout << POLYNOMIAL(4,85, -82, 175, -16, 197) + POLYNOMIAL(4,10, -136, 120, -192, 52) << endl;
cout << POLYNOMIAL(8,183, 141, 66, 9, 57, -121, 65, -93, 143) + POLYNOMIAL(8,120, -162, -135, 31, 80, -78, -200, 123, 129) << endl;
cout << POLYNOMIAL(5,-88, 139, 176, -61, 187, -82) + POLYNOMIAL(5,28, -89, -39, -71, -99, -110) << endl;
cout << POLYNOMIAL(3,99, 4, -181, 32) + POLYNOMIAL(3,103, -147, 109, 7) << endl;
cout << POLYNOMIAL(2,118, -45, -35) + POLYNOMIAL(2,-20, 35, 120) << endl;
cout << POLYNOMIAL(7,-170, -160, -164, -92, 4, 10, 77, -30) + POLYNOMIAL(7,-186, 132, 19, -169, 173, -49, 92, -40) << endl;
cout << POLYNOMIAL(4,112, 6, -122, 187, 113) + POLYNOMIAL(4,25, -43, -149, 160, -103) << endl;
cout << POLYNOMIAL(1,-66, 123) + POLYNOMIAL(1,-194, -197) << endl;
cout << POLYNOMIAL(0,97) + POLYNOMIAL(0,74) << endl;
cout << POLYNOMIAL(0,128) + POLYNOMIAL(0,-1) << endl;
cout << POLYNOMIAL(3,196, -139, 139, 198) + POLYNOMIAL(3,-84, -141, 128, -80) << endl;
cout << POLYNOMIAL(3,170, 194, 177, 135) + POLYNOMIAL(3,12, -77, 48, 142) << endl;
cout << POLYNOMIAL(4,116, -30, -136, 110, -141) + POLYNOMIAL(4,192, -28, -52, 87, 51) << endl;
cout << POLYNOMIAL(6,138, 36, 111, -95, 117, -76, -43) + POLYNOMIAL(6,113, -190, 17, -157, 108, 2, -40) << endl;
cout << POLYNOMIAL(9,-160, -33, 49, 127, -122, 186, 5, 30, 124, -63) + POLYNOMIAL(9,70, -127, -123, -119, 77, 88, 3, -196, -138, -152) << endl;
cout << POLYNOMIAL(9,-160, -135, 63, -97, -73, -55, 195, -185, 17, 166) + POLYNOMIAL(9,-47, -110, -1, -17, 40, 58, 9, -73, -95, 80) << endl;
cout << POLYNOMIAL(5,-131, -19, -112, -116, -197, -90) + POLYNOMIAL(5,128, 28, 108, 92, 9, -36) << endl;
cout << POLYNOMIAL(7,-2, -61, -131, 192, 130, -115, 141, 189) + POLYNOMIAL(7,171, -127, -160, 56, 46, -71, 26, -188) << endl;
cout << POLYNOMIAL(1,-106, 56) + POLYNOMIAL(1,31, 122) << endl;
cout << POLYNOMIAL(8,161, 10, 62, -155, -50, 22, -198, -7, 50) + POLYNOMIAL(8,-167, 1, -54, 20, 41, -72, 182, -161, 60) << endl;
cout << POLYNOMIAL(0,128) + POLYNOMIAL(0,-119) << endl;
cout << POLYNOMIAL(3,-95, -89, 32, -152) + POLYNOMIAL(3,67, 72, 164, 185) << endl;
cout << POLYNOMIAL(7,-29, 50, -7, -200, -172, 103, 129, 173) + POLYNOMIAL(7,-46, 182, -17, -190, -166, -92, 116, 102) << endl;
cout << POLYNOMIAL(3,-94, 78, -150, 41) + POLYNOMIAL(3,59, -179, 160, 25) << endl;
cout << POLYNOMIAL(9,20, 67, 137, 119, -6, -164, -160, 178, -129, -127) + POLYNOMIAL(9,-55, -162, 50, -7, 195, 144, -173, 49, 6, -7) << endl;
cout << POLYNOMIAL(8,-195, -154, 152, -58, -140, -133, -111, -124, 173) + POLYNOMIAL(8,77, -138, 170, -81, 68, 59, -41, 142, 150) << endl;
cout << POLYNOMIAL(8,-87, 3, 166, -194, -162, 165, 81, -139, -99) + POLYNOMIAL(8,-170, -174, 145, -60, 185, -130, -82, -75, 178) << endl;
cout << POLYNOMIAL(8,85, -24, 152, 63, -151, -140, -38, 15, -34) + POLYNOMIAL(8,-24, -94, 61, 34, 44, -47, -113, -105, -190) << endl;
cout << POLYNOMIAL(6,-86, 90, 182, -73, 12, -49, 83) + POLYNOMIAL(6,21, 48, -138, -194, 198, -91, -41) << endl;
cout << POLYNOMIAL(7,-152, 96, -115, -87, 97, -175, -63, 198) + POLYNOMIAL(7,-105, -39, -59, 80, 28, 7, -185, 131) << endl;
cout << POLYNOMIAL(3,46, -25, -115, 67) + POLYNOMIAL(3,-200, 22, 108, -54) << endl;
cout << POLYNOMIAL(3,-167, -198, 58, 35) + POLYNOMIAL(3,-86, 99, 101, 170) << endl;
cout << POLYNOMIAL(1,-38, -169) + POLYNOMIAL(1,-140, 135) << endl;
cout << POLYNOMIAL(4,-76, -135, 14, 46, -121) + POLYNOMIAL(4,-51, -193, 180, -82, -166) << endl;
cout << POLYNOMIAL(1,22, 18) + POLYNOMIAL(1,94, -90) << endl;
cout << POLYNOMIAL(3,9, 187, 103, 20) + POLYNOMIAL(3,140, 166, -131, 150) << endl;
cout << POLYNOMIAL(0,-140) + POLYNOMIAL(0,135) << endl;
cout << POLYNOMIAL(8,92, -14, 52, 29, 23, -144, 133, -71, 54) + POLYNOMIAL(8,128, -87, -29, 132, 37, 30, -139, 174, 43) << endl;
cout << POLYNOMIAL(5,199, 70, -178, 31, 115, -190) + POLYNOMIAL(5,45, 179, -60, -163, 48, 150) << endl;
cout << POLYNOMIAL(0,22) + POLYNOMIAL(0,-124) << endl;
cout << POLYNOMIAL(4,-172, 179, 113, 79, -65) + POLYNOMIAL(4,194, -98, -124, 26, -165) << endl;
cout << POLYNOMIAL(2,188, -128, -45) + POLYNOMIAL(2,6, 133, 84) << endl;
cout << POLYNOMIAL(3,-102, 135, 111, 80) + POLYNOMIAL(3,130, 43, -149, -47) << endl;
cout << POLYNOMIAL(6,-134, 106, -134, 5, 144, -167, 200) + POLYNOMIAL(6,105, -36, 108, -184, 137, 140, -159) << endl;
cout << POLYNOMIAL(7,-147, -42, 196, 159, 183, 24, 46, -153) + POLYNOMIAL(7,51, 20, 48, 115, -188, 87, 78, 62) << endl;
cout << POLYNOMIAL(8,57, -40, 25, -139, 184, -170, 57, 18, 61) + POLYNOMIAL(8,-101, -154, 143, 36, 40, 23, -53, -47, 87) << endl;
cout << POLYNOMIAL(4,-27, 101, -96, -61, 133) + POLYNOMIAL(4,-65, 20, -55, -108, -59) << endl;
cout << POLYNOMIAL(7,119, -114, 6, -31, 170, 75, 123, -66) + POLYNOMIAL(7,121, 61, 82, 196, 19, 128, 87, -64) << endl;
cout << POLYNOMIAL(8,-164, -29, -143, 113, -143, 58, -199, 82, -61) + POLYNOMIAL(8,51, -126, 100, 42, 104, -175, 7, 116, 41) << endl;
cout << POLYNOMIAL(7,-88, 17, 150, 138, 158, 198, -68, -29) + POLYNOMIAL(7,34, -113, 89, -50, 63, 163, -72, -113) << endl;
cout << POLYNOMIAL(8,35, 82, -138, 23, 13, 53, 137, -131, 0) + POLYNOMIAL(8,-198, 21, -180, -15, 44, -187, 159, 75, 73) << endl;
cout << POLYNOMIAL(3,-130, 112, -123, 24) + POLYNOMIAL(3,-49, -100, -119, 140) << endl;
cout << POLYNOMIAL(5,-136, 183, -177, -157, 37, -33) + POLYNOMIAL(5,149, -88, -161, -43, -51, -67) << endl;
cout << POLYNOMIAL(2,-91, 180, -101) + POLYNOMIAL(2,110, -23, -126) << endl;
cout << POLYNOMIAL(9,-78, -70, -62, -110, 153, 187, -137, 197, 23, -91) + POLYNOMIAL(9,148, 73, 85, -4, 159, 127, -59, 114, -73, 23) << endl;
cout << POLYNOMIAL(2,71, -159, 103) + POLYNOMIAL(2,-21, -186, -102) << endl;
cout << POLYNOMIAL(8,108, -96, 144, 158, 78, -49, 191, 83, -28) + POLYNOMIAL(8,-52, -21, -60, 155, 86, -17, 121, -170, -17) << endl;
cout << POLYNOMIAL(4,-11, 164, 121, 40, 52) + POLYNOMIAL(4,152, 62, -142, -172, -109) << endl;
cout << POLYNOMIAL(0,91) + POLYNOMIAL(0,-89) << endl;
cout << POLYNOMIAL(9,-38, 186, 72, 102, -65, -194, -3, 147, -36, 77) + POLYNOMIAL(9,111, -90, -4, -151, -77, 154, -172, 135, -164, -67) << endl;
cout << POLYNOMIAL(9,-98, -116, -44, 6, 44, 27, -6, -165, 123, -107) + POLYNOMIAL(9,74, 118, -144, 164, 134, -131, 20, 79, 65, -154) << endl;
cout << POLYNOMIAL(3,-130, -17, 137, -6) + POLYNOMIAL(3,96, -58, -74, 141) << endl;
cout << POLYNOMIAL(9,93, -198, -10, 51, 147, 193, -12, 184, -73, 35) + POLYNOMIAL(9,56, 178, 105, -116, 56, 63, 167, -49, 185, 114) << endl;
cout << POLYNOMIAL(2,172, 170, 62) + POLYNOMIAL(2,-157, 191, 46) << endl;
cout << POLYNOMIAL(6,-37, -19, 21, 48, -28, 172, -95) + POLYNOMIAL(6,71, 148, -67, -90, -89, -93, 93) << endl;
cout << POLYNOMIAL(1,28, -192) + POLYNOMIAL(1,50, 20) << endl;
cout << POLYNOMIAL(2,16, 23, -125) + POLYNOMIAL(2,-36, 127, -170) << endl;
cout << POLYNOMIAL(9,-179, 108, -79, -123, 7, 64, 16, 26, 102, 195) + POLYNOMIAL(9,-111, -156, 30, 130, 60, 105, -145, 63, -16, 141) << endl;
cout << POLYNOMIAL(9,-22, -123, -110, 5, 60, -66, -2, -194, 189, 101) + POLYNOMIAL(9,125, -22, 45, 76, 33, 60, 193, 106, -6, -145) << endl;
cout << POLYNOMIAL(9,110, -52, 47, -111, 162, -48, 197, -45, 21, -131) + POLYNOMIAL(9,-10, 0, 55, -6, 70, 3, 91, 26, -69, 3) << endl;
cout << POLYNOMIAL(3,-164, 14, -30, -141) + POLYNOMIAL(3,152, 75, 107, 48) << endl;
cout << POLYNOMIAL(0,172) + POLYNOMIAL(0,51) << endl;
cout << POLYNOMIAL(2,78, 42, 152) + POLYNOMIAL(2,-85, 55, -39) << endl;
cout << POLYNOMIAL(4,-88, -134, 124, -58, -164) + POLYNOMIAL(4,112, 18, -141, 75, -15) << endl;
cout << POLYNOMIAL(7,181, 131, -196, -105, -25, -41, 153, -195) + POLYNOMIAL(7,-102, -97, 120, 59, -190, -24, 175, 31) << endl;
cout << POLYNOMIAL(6,188, 159, 24, -144, 171, -122, 47) + POLYNOMIAL(6,151, -183, 96, -172, -17, -78, 47) << endl;
cout << POLYNOMIAL(3,-31, 3, -55, 78) + POLYNOMIAL(3,-160, -63, 107, 195) << endl;
cout << POLYNOMIAL(8,-136, -123, 17, -71, -119, -188, 117, 64, 63) + POLYNOMIAL(8,-61, -42, 46, 100, -95, -94, 171, -141, 79) << endl;
cout << POLYNOMIAL(0,-127) + POLYNOMIAL(0,-182) << endl;
cout << POLYNOMIAL(8,-36, -184, 24, -7, -185, 169, 41, 184, -118) + POLYNOMIAL(8,-193, 151, 100, 196, -77, 33, 73, -30, -103) << endl;
cout << POLYNOMIAL(5,190, -77, 34, 85, 196, -35) + POLYNOMIAL(5,-16, 102, -11, -121, 10, 91) << endl;
cout << POLYNOMIAL(6,-102, -194, -107, 76, 13, -28, -77) + POLYNOMIAL(6,93, -194, 175, 171, -87, -26, -84) << endl;
cout << POLYNOMIAL(6,-81, 35, 24, 145, 19, -124, 188) + POLYNOMIAL(6,157, -179, 29, 45, -97, 126, -25) << endl;
cout << POLYNOMIAL(0,64) + POLYNOMIAL(0,106) << endl;
cout << POLYNOMIAL(3,-121, 14, -102, 177) + POLYNOMIAL(3,-30, -44, -119, -145) << endl;
cout << POLYNOMIAL(0,-111) + POLYNOMIAL(0,-70) << endl;
cout << POLYNOMIAL(3,-78, 150, 69, 56) + POLYNOMIAL(3,-197, -39, -197, 64) << endl;
cout << POLYNOMIAL(7,69, -28, 157, 99, 174, -113, -4, 20) + POLYNOMIAL(7,37, -85, -72, 54, 140, -71, 45, 32) << endl;
cout << POLYNOMIAL(9,-50, 13, 147, -45, 57, 162, -199, 188, 184, -39) + POLYNOMIAL(9,51, -51, 51, 122, -5, -62, -182, -77, -85, -3) << endl;
cout << POLYNOMIAL(7,57, -174, -135, -112, 51, 112, 114, 183) + POLYNOMIAL(7,86, 131, -5, -126, -146, 93, -54, 153) << endl;
cout << POLYNOMIAL(8,123, 164, -167, -99, -117, -1, 105, 133, 45) + POLYNOMIAL(8,59, 114, 145, 114, -144, -113, -71, 44, -118) << endl;
cout << POLYNOMIAL(1,-19, 57) + POLYNOMIAL(1,-37, -103) << endl;
cout << POLYNOMIAL(2,77, -156, 44) + POLYNOMIAL(2,-121, -104, 14) << endl;
cout << POLYNOMIAL(9,131, -188, -39, 137, 82, 129, 96, 160, -122, -127) + POLYNOMIAL(9,-50, -199, -151, -19, -162, 137, 75, -185, 26, -69) << endl;
cout << POLYNOMIAL(4,-171, -139, 93, -134, -74) + POLYNOMIAL(4,-63, -180, 11, -136, -144) << endl;
cout << POLYNOMIAL(7,-134, 130, -114, -153, -6, -38, 26, 114) + POLYNOMIAL(7,172, 12, 14, -125, -90, 176, -53, -51) << endl;
cout << POLYNOMIAL(1,148, 121) + POLYNOMIAL(1,-185, 47) << endl;
cout << POLYNOMIAL(4,-117, -82, 89, -128, -200) + POLYNOMIAL(4,-159, 39, 181, -11, 21) << endl;
cout << POLYNOMIAL(4,3, 12, -181, 134, -80) + POLYNOMIAL(4,-120, -175, -169, 172, -91) << endl;
cout << POLYNOMIAL(2,-107, -169, -125) + POLYNOMIAL(2,-170, 18, 87) << endl;
cout << POLYNOMIAL(4,170, -175, 2, 99, -98) + POLYNOMIAL(4,81, 31, -49, 26, -181) << endl;
cout << POLYNOMIAL(0,144) + POLYNOMIAL(0,67) << endl;
cout << POLYNOMIAL(0,-163) + POLYNOMIAL(0,-136) << endl;
cout << POLYNOMIAL(1,-18, 11) + POLYNOMIAL(1,-122, -7) << endl;
cout << POLYNOMIAL(2,-56, -165, 66) + POLYNOMIAL(2,-11, 123, -69) << endl;
cout << POLYNOMIAL(5,-128, 196, -158, -68, 50, 3) + POLYNOMIAL(5,91, 147, 184, 13, 187, 132) << endl;
cout << POLYNOMIAL(4,-74, 54, -190, -11, -55) + POLYNOMIAL(4,-169, 95, 139, 93, 117) << endl;
cout << POLYNOMIAL(9,-199, 105, 0, 154, -110, -167, 114, -187, 87, 199) + POLYNOMIAL(9,-47, 172, 150, 101, -140, -47, -176, 54, 118, 11) << endl;
cout << POLYNOMIAL(3,12, 199, 105, -59) + POLYNOMIAL(3,156, 134, -43, -199) << endl;
cout << POLYNOMIAL(8,178, -103, 193, -142, 18, -140, 23, 97, -144) + POLYNOMIAL(8,-123, 1, 34, 156, -176, -54, 15, -111, -129) << endl;
cout << POLYNOMIAL(0,-16) + POLYNOMIAL(0,-188) << endl;
cout << POLYNOMIAL(7,-80, 105, 30, -39, -133, 20, 178, 148) + POLYNOMIAL(7,-9, 9, -110, -33, -42, -19, -196, -22) << endl;
cout << POLYNOMIAL(5,103, -169, -117, 168, -24, -6) + POLYNOMIAL(5,-187, -111, 33, -162, -24, 179) << endl;
cout << POLYNOMIAL(6,-184, 191, -54, -197, -48, -24, 20) + POLYNOMIAL(6,-19, 190, 83, -186, 42, -148, -178) << endl;
cout << POLYNOMIAL(9,-57, 6, -155, 61, -46, 100, -78, 64, -7, 3) + POLYNOMIAL(9,183, 36, 83, -38, 163, 194, 95, -135, 126, 52) << endl;
cout << POLYNOMIAL(6,111, -26, 51, 160, 65, -22, 138) + POLYNOMIAL(6,63, -97, 27, -11, -163, -79, -184) << endl;
cout << POLYNOMIAL(5,-195, -90, 8, -115, -121, 45) + POLYNOMIAL(5,-140, 138, -142, -12, -43, 109) << endl;
cout << POLYNOMIAL(8,-33, -78, -166, 65, -8, 115, 46, -128, 100) + POLYNOMIAL(8,81, -166, -135, 165, 50, 96, -39, -140, -45) << endl;
cout << POLYNOMIAL(2,-162, -101, -22) + POLYNOMIAL(2,-137, 52, 63) << endl;
cout << POLYNOMIAL(9,61, -192, -32, -28, -136, 62, -115, 152, -61, -193) + POLYNOMIAL(9,82, -13, 12, -161, 115, -122, -166, 95, 177, -193) << endl;
cout << POLYNOMIAL(2,-188, 117, 164) + POLYNOMIAL(2,76, -132, 30) << endl;
cout << POLYNOMIAL(5,-121, 197, -15, -199, -68, 177) + POLYNOMIAL(5,158, 113, 162, 20, 87, -52) << endl;
cout << POLYNOMIAL(6,-71, 81, 181, 191, 5, 141, 17) + POLYNOMIAL(6,49, 141, -179, -102, -29, -168, -109) << endl;
cout << POLYNOMIAL(6,20, -156, -157, 124, -149, -92, -111) + POLYNOMIAL(6,-122, 191, -94, -133, -150, 184, 27) << endl;
cout << POLYNOMIAL(2,-144, 128, -3) + POLYNOMIAL(2,-63, 77, 104) << endl;
cout << POLYNOMIAL(0,-88) + POLYNOMIAL(0,197) << endl;
cout << POLYNOMIAL(7,195, 87, -196, -118, 96, -37, 27, 167) + POLYNOMIAL(7,-134, -64, 160, 6, -13, -22, 117, -90) << endl;
cout << POLYNOMIAL(9,-113, -153, 109, -67, -13, -58, -123, -25, 87, 149) + POLYNOMIAL(9,188, 2, -103, -109, 1, 93, -28, 89, 100, 189) << endl;
cout << POLYNOMIAL(2,144, -94, -72) + POLYNOMIAL(2,156, -56, -141) << endl;
cout << POLYNOMIAL(9,89, -138, 94, 54, 85, -173, -47, -197, -98, -2) + POLYNOMIAL(9,53, -129, -130, -8, 14, -84, 90, -37, 145, -107) << endl;
cout << POLYNOMIAL(3,165, 29, -96, 79) + POLYNOMIAL(3,91, 66, 164, 0) << endl;
cout << POLYNOMIAL(0,-78) + POLYNOMIAL(0,-53) << endl;
cout << POLYNOMIAL(2,136, -99, -182) + POLYNOMIAL(2,-158, -159, 59) << endl;
cout << POLYNOMIAL(6,-78, 188, 194, 99, -36, 59, -191) + POLYNOMIAL(6,158, -24, -11, 174, -180, 144, 195) << endl;
cout << POLYNOMIAL(4,-26, 177, -88, 84, 104) + POLYNOMIAL(4,176, -36, -25, 159, 39) << endl;
cout << POLYNOMIAL(8,127, 147, 186, 163, 95, -74, -127, -45, -119) + POLYNOMIAL(8,186, -11, 184, -177, -98, -56, 132, 186, -182) << endl;
cout << POLYNOMIAL(3,187, 195, -17, -144) + POLYNOMIAL(3,136, -89, -27, 39) << endl;
cout << POLYNOMIAL(5,172, 140, 117, 60, 137, 81) + POLYNOMIAL(5,7, 22, -38, 22, -194, -112) << endl;
cout << POLYNOMIAL(1,105, -199) + POLYNOMIAL(1,81, 14) << endl;
cout << POLYNOMIAL(8,-13, -132, 189, 176, 58, -119, -128, -108, -39) + POLYNOMIAL(8,118, 49, 177, 32, 52, -117, -118, 173, 181) << endl;
cout << POLYNOMIAL(5,197, 84, 78, -3, 7, -199) + POLYNOMIAL(5,33, -4, -180, -69, -90, -147) << endl;
cout << POLYNOMIAL(8,-97, 6, 34, 114, 139, -84, -40, 95, -180) + POLYNOMIAL(8,-182, -63, 81, -16, 122, 148, -93, 166, 13) << endl;
cout << POLYNOMIAL(8,-71, 39, 193, -134, 39, -182, -49, -77, -56) + POLYNOMIAL(8,106, -74, 88, -193, -183, 68, 183, -22, 47) << endl;
cout << POLYNOMIAL(0,-27) + POLYNOMIAL(0,186) << endl;
cout << POLYNOMIAL(4,-130, -72, -141, 96, 52) + POLYNOMIAL(4,117, -162, -12, -16, 132) << endl;
cout << POLYNOMIAL(5,67, -177, -57, -38, 124, -61) + POLYNOMIAL(5,145, -168, -184, 98, -26, 18) << endl;
cout << POLYNOMIAL(7,161, 109, -111, -50, 185, 137, -53, -194) + POLYNOMIAL(7,-107, -112, 42, 89, 172, -101, 8, -154) << endl;
cout << POLYNOMIAL(3,-175, -78, 162, 32) + POLYNOMIAL(3,-67, 45, -151, -185) << endl;
cout << POLYNOMIAL(0,-152) + POLYNOMIAL(0,-122) << endl;
cout << POLYNOMIAL(5,-79, 97, 54, 158, -114, -156) + POLYNOMIAL(5,188, -53, -11, -39, -100, -183) << endl;
cout << POLYNOMIAL(8,-50, 35, 4, -192, -164, -198, 140, 65, -189) + POLYNOMIAL(8,-34, 14, -130, -162, -20, -35, -42, 69, -178) << endl;
cout << POLYNOMIAL(4,-140, -93, -129, -120, -79) + POLYNOMIAL(4,-50, -149, -112, 190, -96) << endl;
cout << POLYNOMIAL(2,-37, 28, 74) + POLYNOMIAL(2,9, -69, -82) << endl;
cout << POLYNOMIAL(1,191, -168) + POLYNOMIAL(1,117, -114) << endl;
cout << POLYNOMIAL(9,13, -173, 171, 123, -84, 29, 162, 170, -186, -38) + POLYNOMIAL(9,-122, 19, -9, -92, 160, -123, 116, -85, 165, 178) << endl;
cout << POLYNOMIAL(1,142, -87) + POLYNOMIAL(1,-120, 153) << endl;
cout << POLYNOMIAL(5,-60, -191, 56, -32, 5, -175) + POLYNOMIAL(5,-148, 40, -85, -101, 107, 117) << endl;
cout << POLYNOMIAL(7,99, 90, -156, 35, -92, -108, -149, -84) + POLYNOMIAL(7,0, -75, -112, -185, 159, 173, 182, -199) << endl;
cout << POLYNOMIAL(6,186, 180, 0, 99, 137, -82, 87) + POLYNOMIAL(6,-77, 3, -137, 182, 192, 196, -190) << endl;
cout << POLYNOMIAL(3,-140, -186, 169, -52) + POLYNOMIAL(3,177, 108, 58, 74) << endl;
cout << POLYNOMIAL(9,181, -103, 33, 50, -144, -60, 187, 196, -53, -67) + POLYNOMIAL(9,-144, -168, 66, -90, -111, 159, -25, -163, -60, 148) << endl;
cout << POLYNOMIAL(7,59, -163, -52, -122, 54, 72, -97, -58) + POLYNOMIAL(7,-115, 145, 100, 150, -189, -171, -134, 7) << endl;
cout << POLYNOMIAL(4,157, 179, 172, -107, 124) + POLYNOMIAL(4,-85, 23, -44, -62, -20) << endl;
cout << POLYNOMIAL(9,-149, 172, -65, -38, -167, 131, -110, -47, -168, 13) + POLYNOMIAL(9,78, 21, -70, 19, -29, -74, 94, -101, 55, -7) << endl;
cout << POLYNOMIAL(3,191, 153, 125, -162) + POLYNOMIAL(3,149, -125, -68, 64) << endl;
cout << POLYNOMIAL(1,-69, -98) + POLYNOMIAL(1,-184, -58) << endl;
cout << POLYNOMIAL(1,39, -53) + POLYNOMIAL(1,111, -172) << endl;
cout << POLYNOMIAL(6,63, -147, 17, -160, -124, 67, 26) + POLYNOMIAL(6,148, 25, -46, 195, 185, -162, -179) << endl;
cout << POLYNOMIAL(2,-7, -41, 72) + POLYNOMIAL(2,34, 184, -128) << endl;
cout << POLYNOMIAL(6,163, 113, -38, 125, 70, 27, -90) + POLYNOMIAL(6,189, 21, 93, -15, -101, -95, -87) << endl;
cout << POLYNOMIAL(5,-45, -3, -56, -70, 133, -96) + POLYNOMIAL(5,-37, -42, -48, -107, 26, 175) << endl;
cout << POLYNOMIAL(5,198, -95, 190, 186, 78, -164) + POLYNOMIAL(5,100, -24, -150, -74, -86, 59) << endl;
cout << POLYNOMIAL(5,110, -45, -106, -28, 142, 39) + POLYNOMIAL(5,166, 126, -199, -170, -193, -31) << endl;
cout << POLYNOMIAL(6,-183, -1, -138, -43, -161, -151, -54) + POLYNOMIAL(6,-119, 44, 162, 78, -37, 59, 140) << endl;
cout << POLYNOMIAL(9,81, -76, -175, 26, 193, 114, 88, 31, -102, -91) + POLYNOMIAL(9,-7, 63, 111, 21, 94, 6, 8, 69, 163, -70) << endl;
cout << POLYNOMIAL(8,122, 124, 60, -104, 129, -30, 151, -130, -36) + POLYNOMIAL(8,-71, -16, 45, -34, -4, 37, -197, -113, -49) << endl;
cout << POLYNOMIAL(8,-177, -149, -166, 82, 27, 43, 145, -30, -100) + POLYNOMIAL(8,-25, 124, 168, 129, 89, -169, 132, -145, 143) << endl;
cout << POLYNOMIAL(0,-60) + POLYNOMIAL(0,106) << endl;
cout << POLYNOMIAL(7,36, 97, 164, 40, -33, 35, -115, 183) + POLYNOMIAL(7,-9, 63, -161, -151, -36, -101, -87, -9) << endl;
cout << POLYNOMIAL(8,-105, 40, -197, 198, 73, -76, -6, 106, 26) + POLYNOMIAL(8,-191, 120, -121, 158, 152, -90, -176, 187, -184) << endl;
cout << POLYNOMIAL(3,52, -170, -200, 197) + POLYNOMIAL(3,59, 193, 146, 109) << endl;
cout << POLYNOMIAL(9,-45, 110, -81, 40, -171, 188, -8, 136, 147, -6) + POLYNOMIAL(9,-42, 154, 196, 28, 50, 89, -69, -21, -99, -86) << endl;
cout << POLYNOMIAL(3,-115, -41, -13, 88) + POLYNOMIAL(3,134, 200, -154, -161) << endl;
cout << POLYNOMIAL(4,-76, 113, -69, -114, 101) + POLYNOMIAL(4,-61, -41, -56, -64, -59) << endl;
cout << POLYNOMIAL(3,30, -73, -17, 147) + POLYNOMIAL(3,-52, -171, 1, 47) << endl;
cout << POLYNOMIAL(8,190, -37, -42, -172, 164, 83, 176, -183, 184) + POLYNOMIAL(8,-160, 90, -181, 85, 38, -80, 15, -9, -92) << endl;
cout << POLYNOMIAL(4,-130, 157, -17, 126, 88) + POLYNOMIAL(4,188, -25, -176, -70, 50) << endl;
cout << POLYNOMIAL(3,97, 76, 198, -54) + POLYNOMIAL(3,175, -149, -171, -67) << endl;
cout << POLYNOMIAL(1,92, -125) + POLYNOMIAL(1,-164, -74) << endl;
cout << POLYNOMIAL(4,96, 60, 96, 188, 199) + POLYNOMIAL(4,-117, -69, 65, -15, 46) << endl;
cout << POLYNOMIAL(3,-146, 90, -82, 43) + POLYNOMIAL(3,169, -125, -50, -8) << endl;
cout << POLYNOMIAL(2,52, 28, 71) + POLYNOMIAL(2,9, -97, -2) << endl;
cout << POLYNOMIAL(2,124, -152, 196) + POLYNOMIAL(2,31, -114, 59) << endl;
cout << POLYNOMIAL(7,-28, -65, -148, 119, 20, 74, -123, 126) + POLYNOMIAL(7,-150, 101, -104, 105, -146, -75, 60, -133) << endl;
cout << POLYNOMIAL(7,-186, -81, 60, -152, -63, 51, 170, 155) + POLYNOMIAL(7,-6, 139, -145, 173, 108, 81, 5, 165) << endl;
cout << POLYNOMIAL(2,82, 87, 149) + POLYNOMIAL(2,-120, -159, 175) << endl;
cout << POLYNOMIAL(2,-179, 115, -18) + POLYNOMIAL(2,124, -123, -42) << endl;
cout << POLYNOMIAL(9,-188, 24, 183, -24, -50, -21, -180, 151, -121, 119) + POLYNOMIAL(9,-89, -140, -142, -135, -87, 50, -37, -100, 161, -11) << endl;
cout << POLYNOMIAL(2,32, 14, 76) + POLYNOMIAL(2,-35, 34, 83) << endl;
cout << POLYNOMIAL(6,-19, 106, -59, -68, 125, 79, 14) + POLYNOMIAL(6,-192, 5, -113, 8, 183, 105, -95) << endl;
cout << POLYNOMIAL(5,105, -22, -75, 13, -112, 158) + POLYNOMIAL(5,-18, -161, 48, -87, -93, -192) << endl;
cout << POLYNOMIAL(3,-67, -105, 155, -35) + POLYNOMIAL(3,-7, 65, -30, -84) << endl;
cout << POLYNOMIAL(3,-12, 135, 116, 37) + POLYNOMIAL(3,33, 184, -102, 38) << endl;
cout << POLYNOMIAL(9,-163, 36, 187, -20, -154, 76, 132, 19, -100, -185) + POLYNOMIAL(9,-124, -6, 66, -163, 85, 64, -187, 118, 20, 17) << endl;
cout << POLYNOMIAL(4,74, 73, 44, 130, -197) + POLYNOMIAL(4,-124, 50, 45, 179, -52) << endl;
cout << POLYNOMIAL(3,-145, 68, 119, -36) + POLYNOMIAL(3,184, -13, -181, 198) << endl;
cout << POLYNOMIAL(4,21, 53, 70, 116, 5) + POLYNOMIAL(4,108, -61, 100, 76, 2) << endl;
cout << POLYNOMIAL(7,-66, -35, 142, -30, 170, -27, 55, -147) + POLYNOMIAL(7,32, 28, -163, 117, 115, -130, -123, -98) << endl;
cout << POLYNOMIAL(0,-186) + POLYNOMIAL(0,193) << endl;
cout << POLYNOMIAL(9,21, -58, -168, 99, -190, 156, -164, 166, 17, 2) + POLYNOMIAL(9,-130, 192, 14, 135, -75, -68, -160, -117, 13, -64) << endl;
cout << POLYNOMIAL(2,-177, 83, -11) + POLYNOMIAL(2,138, -160, 73) << endl;
cout << POLYNOMIAL(0,-125) + POLYNOMIAL(0,29) << endl;
cout << POLYNOMIAL(7,44, -99, -91, -5, -180, 120, 178, -138) + POLYNOMIAL(7,161, -9, 140, 167, -50, 9, -198, -118) << endl;
cout << POLYNOMIAL(1,-32, -140) + POLYNOMIAL(1,-16, -177) << endl;
cout << POLYNOMIAL(1,0, 161) + POLYNOMIAL(1,41, 168) << endl;
cout << POLYNOMIAL(3,-74, -8, 38, 68) + POLYNOMIAL(3,-124, -28, -185, -36) << endl;
cout << POLYNOMIAL(0,-145) + POLYNOMIAL(0,158) << endl;
cout << POLYNOMIAL(4,59, 7, 54, 16, 173) + POLYNOMIAL(4,108, 134, -141, 173, 72) << endl;
cout << POLYNOMIAL(9,-96, -3, -50, -60, 8, -143, -42, -77, -31, -160) + POLYNOMIAL(9,19, 188, 165, -23, -97, -37, -81, -25, -72, 101) << endl;
cout << POLYNOMIAL(4,-35, 47, -165, 75, 61) + POLYNOMIAL(4,121, 166, 200, 154, -169) << endl;
cout << POLYNOMIAL(5,152, -25, 33, -192, -153, 55) + POLYNOMIAL(5,-107, 44, 180, 55, -194, -118) << endl;
cout << POLYNOMIAL(0,-168) + POLYNOMIAL(0,-145) << endl;
cout << POLYNOMIAL(0,27) + POLYNOMIAL(0,142) << endl;
cout << POLYNOMIAL(1,-48, -135) + POLYNOMIAL(1,90, 50) << endl;
cout << POLYNOMIAL(3,45, 182, 156, -167) + POLYNOMIAL(3,-99, -100, -124, 170) << endl;
cout << POLYNOMIAL(9,44, -161, -12, 181, 18, -116, 54, 195, 31, 129) + POLYNOMIAL(9,34, -180, -42, -15, 107, 9, -185, 175, -188, -161) << endl;
cout << POLYNOMIAL(0,12) + POLYNOMIAL(0,114) << endl;
cout << POLYNOMIAL(8,161, -200, -37, -189, 157, -79, 77, 180, -147) + POLYNOMIAL(8,-134, -80, -197, 37, 125, 159, 76, 197, -148) << endl;
cout << POLYNOMIAL(8,-199, 148, -107, -129, -161, 101, 140, -196, -24) + POLYNOMIAL(8,92, -25, 72, -55, -74, -120, -41, -28, 43) << endl;
cout << POLYNOMIAL(8,187, -24, 15, -71, 108, 130, 40, 152, 87) + POLYNOMIAL(8,-108, -170, -187, -60, 43, -79, -62, -165, -17) << endl;
cout << POLYNOMIAL(3,-85, 169, -11, -100) + POLYNOMIAL(3,124, -196, 191, 162) << endl;
cout << POLYNOMIAL(6,60, 31, -189, 137, 112, 174, 73) + POLYNOMIAL(6,100, -157, 44, 117, -69, -151, 134) << endl;
cout << POLYNOMIAL(5,132, -70, -88, -15, 139, 2) + POLYNOMIAL(5,-142, 184, -31, 145, -199, 72) << endl;
cout << POLYNOMIAL(8,89, -170, 125, -43, 125, 43, 8, -195, 132) + POLYNOMIAL(8,169, -191, -109, -6, -81, -193, 171, 194, 163) << endl;
cout << POLYNOMIAL(8,-146, 69, 4, 197, -154, -94, 117, -3, 133) + POLYNOMIAL(8,59, -69, 129, 4, -46, 173, 138, 114, -187) << endl;
cout << POLYNOMIAL(5,-37, 169, 173, 63, -115, -188) + POLYNOMIAL(5,-54, -111, 122, 38, 171, -142) << endl;
cout << POLYNOMIAL(5,-66, -113, 156, 2, -180, -61) + POLYNOMIAL(5,-180, 158, 55, -164, 27, 34) << endl;
cout << POLYNOMIAL(5,31, 175, -115, 14, 13, -153) + POLYNOMIAL(5,138, -162, 146, -2, -97, -77) << endl;
cout << POLYNOMIAL(6,-5, -113, 39, 195, -4, 62, 30) + POLYNOMIAL(6,-155, -38, -89, 91, -192, -58, -81) << endl;
cout << POLYNOMIAL(7,35, -25, 191, 24, -159, 74, -44, 113) + POLYNOMIAL(7,-98, 120, -71, -140, -172, 22, -84, 192) << endl;
cout << POLYNOMIAL(7,-69, 36, -35, 97, -46, -40, 114, 184) + POLYNOMIAL(7,197, 12, -58, -4, 165, -7, -95, 82) << endl;
cout << POLYNOMIAL(5,-151, -19, 9, -174, 63, 159) + POLYNOMIAL(5,109, 132, 114, -115, 33, 69) << endl;
cout << POLYNOMIAL(2,184, -48, 11) + POLYNOMIAL(2,61, -27, -113) << endl;
cout << POLYNOMIAL(4,90, -169, 44, -92, 104) + POLYNOMIAL(4,-6, 180, 157, 159, -152) << endl;
cout << POLYNOMIAL(6,-141, -121, 184, -108, 60, 178, -79) + POLYNOMIAL(6,111, 34, -97, 188, 134, -178, 198) << endl;
cout << POLYNOMIAL(1,191, -151) + POLYNOMIAL(1,47, -53) << endl;
cout << POLYNOMIAL(9,99, 23, 55, -115, 76, 159, 38, 69, -43, 12) + POLYNOMIAL(9,87, 114, -45, 77, 140, 166, -20, -72, 33, 10) << endl;
cout << POLYNOMIAL(5,133, 169, 2, -167, -132, 118) + POLYNOMIAL(5,-183, 1, -145, -171, -62, -125) << endl;
cout << POLYNOMIAL(3,-165, -158, 132, 158) + POLYNOMIAL(3,69, -192, 47, -116) << endl;
cout << POLYNOMIAL(5,50, 126, -122, 115, 130, -54) + POLYNOMIAL(5,-172, -188, -118, -188, -89, 36) << endl;
cout << POLYNOMIAL(7,-5, 149, 63, 140, -123, 9, 41, 40) + POLYNOMIAL(7,7, -63, -41, -111, 23, -23, 181, 51) << endl;
cout << POLYNOMIAL(4,-22, 9, -34, 58, -69) + POLYNOMIAL(4,179, -53, -196, -162, -162) << endl;
cout << POLYNOMIAL(7,114, 187, -110, 198, -127, -30, 68, 169) + POLYNOMIAL(7,113, -179, -19, 5, -168, -105, -140, 26) << endl;
cout << POLYNOMIAL(0,175) + POLYNOMIAL(0,-174) << endl;
cout << POLYNOMIAL(0,-8) + POLYNOMIAL(0,-196) << endl;
cout << POLYNOMIAL(0,133) + POLYNOMIAL(0,115) << endl;
cout << POLYNOMIAL(1,-86, 190) + POLYNOMIAL(1,-161, -124) << endl;
cout << POLYNOMIAL(0,-35) + POLYNOMIAL(0,-43) << endl;
cout << POLYNOMIAL(3,65, -186, 60, -96) + POLYNOMIAL(3,-21, 141, 70, -198) << endl;
cout << POLYNOMIAL(7,-107, -169, 103, 104, 40, 145, -71, 191) + POLYNOMIAL(7,157, -187, 190, 30, -184, -11, -38, -30) << endl;
cout << POLYNOMIAL(2,-116, 109, 65) + POLYNOMIAL(2,158, -78, -178) << endl;
cout << POLYNOMIAL(6,154, -193, -154, 162, -170, -177, 25) + POLYNOMIAL(6,79, 119, -149, 74, -161, -189, 148) << endl;
cout << POLYNOMIAL(4,9, 53, 42, 59, 164) + POLYNOMIAL(4,53, -17, -196, 111, 191) << endl;
cout << POLYNOMIAL(7,-192, -69, -64, 134, 60, -80, 15, -18) + POLYNOMIAL(7,130, 145, -164, 160, 190, -168, 189, 25) << endl;
cout << POLYNOMIAL(5,-144, -187, -71, -17, -48, 113) + POLYNOMIAL(5,163, -49, 43, -87, -137, -108) << endl;
cout << POLYNOMIAL(9,-190, 163, -94, -200, -45, 169, 59, -152, 178, 181) + POLYNOMIAL(9,162, -143, -16, 62, -116, 18, -64, -148, -30, -185) << endl;
cout << POLYNOMIAL(6,190, 68, -109, -40, -154, 165, -93) + POLYNOMIAL(6,-196, 58, 102, 28, -174, -76, -82) << endl;
cout << POLYNOMIAL(8,168, -125, -98, 44, 136, -163, 182, 13, -25) + POLYNOMIAL(8,-130, -133, 145, 149, -48, 19, -187, 18, 5) << endl;
cout << POLYNOMIAL(9,-47, -174, -57, 85, -3, 91, -162, 117, 22, 56) + POLYNOMIAL(9,-155, -11, 160, -71, 10, 118, -85, -86, -92, -43) << endl;
cout << POLYNOMIAL(3,-66, -193, -58, 158) + POLYNOMIAL(3,91, 132, -138, -128) << endl;
cout << POLYNOMIAL(4,26, -101, 119, -14, -87) + POLYNOMIAL(4,-109, 152, -50, -176, -23) << endl;
cout << POLYNOMIAL(2,95, -100, -22) + POLYNOMIAL(2,-129, 108, -60) << endl;
cout << POLYNOMIAL(4,170, -86, -103, 165, 89) + POLYNOMIAL(4,55, 49, 104, 0, -133) << endl;
cout << POLYNOMIAL(0,114) + POLYNOMIAL(0,104) << endl;
cout << POLYNOMIAL(7,-46, 165, 92, -19, 186, 181, -1, 137) + POLYNOMIAL(7,-141, 117, 127, -161, 44, -25, -48, 135) << endl;
cout << POLYNOMIAL(0,7) + POLYNOMIAL(0,48) << endl;
cout << POLYNOMIAL(9,-188, -23, -86, -104, -100, -80, 152, 18, 123, -84) + POLYNOMIAL(9,-120, 109, 72, 72, -123, -105, -29, -109, -118, -195) << endl;
cout << POLYNOMIAL(3,18, -185, -132, -7) + POLYNOMIAL(3,11, -63, 15, 77) << endl;
cout << POLYNOMIAL(4,-15, -34, -83, 16, -62) + POLYNOMIAL(4,159, -18, -20, -16, 104) << endl;
cout << POLYNOMIAL(7,-107, 154, 34, -162, 56, -80, -184, 198) + POLYNOMIAL(7,-91, -116, -178, -112, 117, -9, 123, 146) << endl;
cout << POLYNOMIAL(2,99, 61, 165) + POLYNOMIAL(2,32, -142, -8) << endl;
cout << POLYNOMIAL(8,163, -126, -171, -194, -193, -182, -7, -84, 29) + POLYNOMIAL(8,-48, 41, 50, -125, 164, -163, -61, -157, 8) << endl;
cout << POLYNOMIAL(5,-161, 35, 86, 70, -165, -194) + POLYNOMIAL(5,-84, 144, 33, 41, 194, -5) << endl;
cout << POLYNOMIAL(0,107) + POLYNOMIAL(0,136) << endl;
cout << POLYNOMIAL(2,-47, 147, 9) + POLYNOMIAL(2,51, 163, 86) << endl;
cout << POLYNOMIAL(2,117, 165, 150) + POLYNOMIAL(2,68, 150, 183) << endl;
cout << POLYNOMIAL(8,-13, 7, -28, 26, -37, -95, 10, -13, -171) + POLYNOMIAL(8,168, 128, -177, -61, 165, 160, -89, 10, 188) << endl;
cout << POLYNOMIAL(2,-164, 172, 171) + POLYNOMIAL(2,-113, 93, -164) << endl;
cout << POLYNOMIAL(7,122, 92, 26, -139, -138, -32, -51, 119) + POLYNOMIAL(7,189, 130, 76, 87, -107, -3, 143, 24) << endl;
cout << POLYNOMIAL(3,28, 14, -25, -11) + POLYNOMIAL(3,33, 4, 181, 102) << endl;
cout << POLYNOMIAL(2,15, 170, -19) + POLYNOMIAL(2,85, -125, -150) << endl;
cout << POLYNOMIAL(6,-186, -20, 94, 80, -194, -84, 118) + POLYNOMIAL(6,174, -152, -47, 137, -137, 113, -93) << endl;
cout << POLYNOMIAL(5,137, -184, 145, 45, -41, 190) + POLYNOMIAL(5,-113, -119, 177, -182, 63, -40) << endl;
cout << POLYNOMIAL(8,136, -67, 117, 91, 182, 175, 44, 45, -54) + POLYNOMIAL(8,-124, -74, 30, -164, 10, 103, -197, -83, -89) << endl;
cout << POLYNOMIAL(8,150, 83, -88, 72, -19, 159, 182, 54, -187) + POLYNOMIAL(8,95, 159, -38, 52, 173, -144, 2, 99, -43) << endl;
cout << POLYNOMIAL(2,-43, 171, 4) + POLYNOMIAL(2,-19, -111, 159) << endl;
cout << POLYNOMIAL(8,-21, 86, -120, 106, -76, 75, 63, -56, -54) + POLYNOMIAL(8,-168, -137, 79, 124, 36, -186, 130, 5, -101) << endl;
cout << POLYNOMIAL(2,9, -56, -162) + POLYNOMIAL(2,-129, 164, -144) << endl;
cout << POLYNOMIAL(8,-90, -30, -13, 134, -40, -8, 50, -92, -11) + POLYNOMIAL(8,-127, -154, 147, -135, 144, -75, 130, -15, -123) << endl;
cout << POLYNOMIAL(1,-76, 60) + POLYNOMIAL(1,-184, 85) << endl;
cout << POLYNOMIAL(0,81) + POLYNOMIAL(0,-173) << endl;
cout << POLYNOMIAL(0,-10) + POLYNOMIAL(0,133) << endl;
cout << POLYNOMIAL(0,115) + POLYNOMIAL(0,115) << endl;
cout << POLYNOMIAL(0,-8) + POLYNOMIAL(0,175) << endl;
cout << POLYNOMIAL(5,-175, 139, 29, -121, 180, 198) + POLYNOMIAL(5,-169, 33, -150, 41, -3, 185) << endl;
cout << POLYNOMIAL(0,33) + POLYNOMIAL(0,51) << endl;
cout << POLYNOMIAL(4,-15, -20, 136, 79, -69) + POLYNOMIAL(4,111, -161, -122, -45, -150) << endl;
cout << POLYNOMIAL(3,167, -146, 72, -56) + POLYNOMIAL(3,69, 138, -138, 162) << endl;
cout << POLYNOMIAL(1,-20, -143) + POLYNOMIAL(1,161, -61) << endl;
cout << POLYNOMIAL(5,191, 126, -40, -172, -25, -119) + POLYNOMIAL(5,89, -111, 175, -33, 147, 31) << endl;
cout << POLYNOMIAL(8,-143, 172, -174, 45, 18, -90, 68, -102, 48) + POLYNOMIAL(8,35, -125, -13, -194, 146, -158, -186, 13, 146) << endl;
cout << POLYNOMIAL(4,166, 23, -187, 35, -50) + POLYNOMIAL(4,-18, 14, -194, 159, -159) << endl;
cout << POLYNOMIAL(1,-122, -93) + POLYNOMIAL(1,-17, -44) << endl;
cout << POLYNOMIAL(5,156, 127, 14, -96, 183, 151) + POLYNOMIAL(5,-194, -186, -3, 87, 128, -118) << endl;
cout << POLYNOMIAL(7,-131, 141, 55, -9, 147, -185, -194, -50) + POLYNOMIAL(7,-132, -78, 84, 95, 186, 27, -146, 176) << endl;
cout << POLYNOMIAL(9,-28, -84, -35, 171, 183, -117, 95, -29, 124, 46) + POLYNOMIAL(9,14, -196, 124, 120, 67, 125, -144, 77, -146, 18) << endl;
cout << POLYNOMIAL(1,90, -79) + POLYNOMIAL(1,30, -8) << endl;
cout << POLYNOMIAL(4,72, -171, 37, 150, -171) + POLYNOMIAL(4,-5, -141, 2, 10, -47) << endl;
cout << POLYNOMIAL(0,-1) + POLYNOMIAL(0,145) << endl;
cout << POLYNOMIAL(6,6, -56, 6, -147, -175, 43, -14) + POLYNOMIAL(6,125, 147, -160, -59, 156, -96, -86) << endl;
cout << POLYNOMIAL(2,165, 161, 41) + POLYNOMIAL(2,194, -56, 199) << endl;
cout << POLYNOMIAL(1,-148, 189) + POLYNOMIAL(1,152, -124) << endl;
cout << POLYNOMIAL(7,197, 86, 159, 155, 137, -94, 102, 86) + POLYNOMIAL(7,172, 112, -63, 87, -192, -95, -53, 100) << endl;
cout << POLYNOMIAL(3,66, 112, 187, -101) + POLYNOMIAL(3,-24, 78, 8, -64) << endl;
cout << POLYNOMIAL(9,82, -90, 56, -129, -66, 154, 85, -82, 111, -144) + POLYNOMIAL(9,-138, 22, 172, 118, -158, 151, 41, -30, -38, 87) << endl;
cout << POLYNOMIAL(2,-144, -30, -179) + POLYNOMIAL(2,-1, 88, -157) << endl;
cout << POLYNOMIAL(4,189, 177, -115, -81, 4) + POLYNOMIAL(4,104, 186, 84, 77, -177) << endl;
cout << POLYNOMIAL(5,-129, -158, -114, 137, 199, -86) + POLYNOMIAL(5,-153, -167, 48, 134, -88, -103) << endl;
cout << POLYNOMIAL(5,2, -132, -38, -93, -113, 50) + POLYNOMIAL(5,-71, -47, -129, 160, 187, -102) << endl;
cout << POLYNOMIAL(6,-109, -95, -138, 62, 29, 86, -18) + POLYNOMIAL(6,186, 184, 149, -70, -127, 178, -48) << endl;
cout << POLYNOMIAL(6,13, 109, -35, -179, -36, 198, -49) + POLYNOMIAL(6,69, 189, -138, -81, -166, 187, -37) << endl;
cout << POLYNOMIAL(2,112, -160, -20) + POLYNOMIAL(2,-77, -166, -92) << endl;
cout << POLYNOMIAL(4,-35, -197, 114, -175, -153) + POLYNOMIAL(4,98, 183, 196, -58, 199) << endl;
cout << POLYNOMIAL(8,175, -120, -132, 130, -10, 167, -113, 14, -148) + POLYNOMIAL(8,-132, 107, -134, 121, 43, 66, 94, -155, -106) << endl;
cout << POLYNOMIAL(1,-15, -150) + POLYNOMIAL(1,22, -183) << endl;
cout << POLYNOMIAL(6,-190, -126, 171, -7, 13, 149, -144) + POLYNOMIAL(6,-132, 200, 87, 109, 30, 95, 59) << endl;
cout << POLYNOMIAL(2,94, -39, -122) + POLYNOMIAL(2,121, -67, 149) << endl;
cout << POLYNOMIAL(8,69, -110, -66, -104, -30, -34, -4, -156, -176) + POLYNOMIAL(8,118, 39, -91, -167, 164, 174, 94, -42, -38) << endl;
cout << POLYNOMIAL(4,90, 61, 103, -27, 46) + POLYNOMIAL(4,94, -116, -45, 149, -200) << endl;
cout << POLYNOMIAL(7,-150, 146, 142, 173, 176, 187, 121, 175) + POLYNOMIAL(7,21, -187, 86, 16, -15, 120, 92, -25) << endl;
cout << POLYNOMIAL(6,-26, -104, 61, -34, -84, 130, 134) + POLYNOMIAL(6,139, -76, -111, 119, 170, 143, -183) << endl;
cout << POLYNOMIAL(8,1, 64, 34, -55, -146, 155, -123, 38, 153) + POLYNOMIAL(8,57, -148, -181, 189, -199, 6, 68, -87, -127) << endl;
cout << POLYNOMIAL(5,-188, -198, 101, 117, 124, 39) + POLYNOMIAL(5,123, 156, -195, 41, -179, -157) << endl;
cout << POLYNOMIAL(0,-154) + POLYNOMIAL(0,9) << endl;
cout << POLYNOMIAL(2,190, -190, -132) + POLYNOMIAL(2,18, 154, -136) << endl;
cout << POLYNOMIAL(2,-155, 78, -137) + POLYNOMIAL(2,-152, -98, -19) << endl;
cout << POLYNOMIAL(4,48, 99, 44, 55, -111) + POLYNOMIAL(4,-181, -71, 141, 137, 58) << endl;
cout << POLYNOMIAL(3,-123, -94, -121, -18) + POLYNOMIAL(3,-119, -24, 144, -52) << endl;
cout << POLYNOMIAL(7,40, 124, -13, -200, -11, 73, 112, -163) + POLYNOMIAL(7,-19, -14, 139, -197, 1, -130, -55, -189) << endl;
cout << POLYNOMIAL(8,-13, 122, 22, -167, -172, 84, 41, -28, 96) + POLYNOMIAL(8,83, -101, 91, -44, -112, -46, 135, -185, -197) << endl;
cout << POLYNOMIAL(4,-102, 195, 124, 148, -116) + POLYNOMIAL(4,128, -172, 117, -183, -193) << endl;
cout << POLYNOMIAL(2,84, 150, 90) + POLYNOMIAL(2,-92, -3, 35) << endl;
cout << POLYNOMIAL(7,-52, 133, 105, -13, -161, 54, -163, 4) + POLYNOMIAL(7,-146, -111, -40, -79, 193, 63, 138, 125) << endl;
cout << POLYNOMIAL(6,-17, -45, 122, 192, 25, 76, -77) + POLYNOMIAL(6,-162, 169, -74, -193, -198, 13, -79) << endl;
cout << POLYNOMIAL(6,-73, -190, -84, 179, 53, 98, -87) + POLYNOMIAL(6,62, -53, -107, -177, -149, -196, 197) << endl;
cout << POLYNOMIAL(7,21, -48, -8, -35, 199, -117, -105, -176) + POLYNOMIAL(7,155, -32, -162, 112, 0, -8, 73, -48) << endl;
cout << POLYNOMIAL(4,140, -110, -65, -196, -176) + POLYNOMIAL(4,20, 66, -124, 24, 186) << endl;
cout << POLYNOMIAL(9,-81, -53, -47, 64, 122, 105, 167, -39, 89, -112) + POLYNOMIAL(9,110, -155, 82, 45, -103, -4, 60, 174, -196, 93) << endl;
cout << POLYNOMIAL(9,-58, 29, -32, 155, -189, 48, -54, -174, -66, -90) + POLYNOMIAL(9,151, 197, 70, -54, -164, -151, 40, -26, 56, 132) << endl;
cout << POLYNOMIAL(9,-64, 171, 17, 74, 45, -168, -97, 0, -71, 87) + POLYNOMIAL(9,26, 109, -156, 62, -32, 162, 135, -19, 150, -51) << endl;
cout << POLYNOMIAL(6,164, -20, -29, 131, -164, -120, 169) + POLYNOMIAL(6,-133, 48, -122, 144, 102, 36, 189) << endl;
cout << POLYNOMIAL(9,151, -194, -47, -29, -107, 45, 162, 17, -11, -89) + POLYNOMIAL(9,37, -188, 18, -16, -102, -25, -64, -89, -138, 143) << endl;
cout << POLYNOMIAL(3,99, 98, -126, 96) + POLYNOMIAL(3,-112, -70, -44, 5) << endl;
cout << POLYNOMIAL(2,36, 36, -3) + POLYNOMIAL(2,-1, 26, 130) << endl;
cout << POLYNOMIAL(6,-190, 40, -102, 167, -161, -87, -65) + POLYNOMIAL(6,180, 78, -125, 23, -159, 183, -89) << endl;
cout << POLYNOMIAL(7,167, 194, -25, -143, -21, 5, -141, 91) + POLYNOMIAL(7,91, 187, 135, -143, 68, -103, 48, 15) << endl;
cout << POLYNOMIAL(1,161, -156) + POLYNOMIAL(1,-90, 22) << endl;
cout << POLYNOMIAL(5,-159, -119, 152, -12, 74, -70) + POLYNOMIAL(5,-23, 169, -125, -181, 98, 38) << endl;
cout << POLYNOMIAL(3,-49, 52, 76, 64) + POLYNOMIAL(3,79, 3, 21, -17) << endl;
cout << POLYNOMIAL(0,-45) + POLYNOMIAL(0,31) << endl;
cout << POLYNOMIAL(3,144, 163, -147, 90) + POLYNOMIAL(3,-18, 126, 16, 27) << endl;
cout << POLYNOMIAL(5,-37, -115, -121, -107, -74, 52) + POLYNOMIAL(5,1, -91, 165, 155, 5, 84) << endl;
cout << POLYNOMIAL(2,-191, -148, -69) + POLYNOMIAL(2,-42, -114, -94) << endl;
cout << POLYNOMIAL(3,130, 20, 67, -46) + POLYNOMIAL(3,-93, 183, 13, -124) << endl;
cout << POLYNOMIAL(4,148, 128, 136, -133, -138) + POLYNOMIAL(4,153, -174, 130, 172, 79) << endl;
cout << POLYNOMIAL(0,-70) + POLYNOMIAL(0,-62) << endl;
cout << POLYNOMIAL(2,-85, -57, -64) + POLYNOMIAL(2,-59, 22, 181) << endl;
cout << POLYNOMIAL(4,-102, 77, -15, 168, 63) + POLYNOMIAL(4,73, 114, -8, 93, 55) << endl;
cout << POLYNOMIAL(9,-186, -172, 148, 58, -186, -178, 91, 174, 105, 183) + POLYNOMIAL(9,-108, -57, -108, 192, -34, -107, 53, -111, 149, 163) << endl;
cout << POLYNOMIAL(6,85, -194, -75, -66, -2, 129, 74) + POLYNOMIAL(6,-189, 156, -38, 198, -74, 101, -88) << endl;
cout << POLYNOMIAL(0,-4) + POLYNOMIAL(0,133) << endl;
cout << POLYNOMIAL(8,-195, -193, 149, -36, 77, 99, 3, -115, -101) + POLYNOMIAL(8,-106, 177, -41, 87, 76, 52, 186, 64, 66) << endl;
cout << POLYNOMIAL(0,-94) + POLYNOMIAL(0,-131) << endl;
cout << POLYNOMIAL(8,102, -68, -99, -141, -164, -57, -167, 9, -25) + POLYNOMIAL(8,133, 26, 56, -195, 128, 199, -101, -105, -33) << endl;
cout << POLYNOMIAL(0,192) + POLYNOMIAL(0,165) << endl;
cout << POLYNOMIAL(9,-94, 196, -126, -167, 182, 140, 72, -25, -157, 10) + POLYNOMIAL(9,-180, -175, -114, -127, -188, 194, 46, -90, -43, -92) << endl;
cout << POLYNOMIAL(8,-126, 192, 46, -105, 41, -37, -121, 149, 83) + POLYNOMIAL(8,128, 149, 15, 199, -144, 196, -129, -21, 101) << endl;
cout << POLYNOMIAL(6,-147, 199, -102, -113, -42, 46, 148) + POLYNOMIAL(6,46, -5, 96, -58, -68, -6, 128) << endl;
cout << POLYNOMIAL(4,-141, 181, -34, 86, 54) + POLYNOMIAL(4,91, 196, -86, -12, 121) << endl;
cout << POLYNOMIAL(1,-73, 32) + POLYNOMIAL(1,-191, -198) << endl;
cout << POLYNOMIAL(7,162, 103, -49, 3, -148, -140, 184, -190) + POLYNOMIAL(7,-179, 183, -111, -99, 90, -36, -189, 195) << endl;
cout << POLYNOMIAL(1,-112, -117) + POLYNOMIAL(1,-142, 145) << endl;
cout << POLYNOMIAL(4,-41, -121, 89, 64, 8) + POLYNOMIAL(4,80, -30, -87, 18, -135) << endl;
cout << POLYNOMIAL(6,184, 171, 30, -5, -151, -7, 79) + POLYNOMIAL(6,22, -128, 19, -198, -95, 35, -154) << endl;
cout << POLYNOMIAL(4,-101, 2, -156, -29, -123) + POLYNOMIAL(4,-97, -59, 8, -67, -128) << endl;
cout << POLYNOMIAL(4,29, 138, -95, 186, 119) + POLYNOMIAL(4,-120, 172, -68, -70, -55) << endl;
cout << POLYNOMIAL(7,93, 148, -154, -36, 175, -95, 44, -22) + POLYNOMIAL(7,182, 197, -74, -106, -174, -62, -80, 134) << endl;
cout << POLYNOMIAL(7,-98, 16, 187, -120, 32, -46, 60, 132) + POLYNOMIAL(7,-51, -114, -189, 164, -49, -67, -119, -81) << endl;
cout << POLYNOMIAL(4,115, -170, 171, 55, 105) + POLYNOMIAL(4,135, 18, -29, -138, -132) << endl;
cout << POLYNOMIAL(8,157, -176, -86, 164, -57, 44, -158, -94, 47) + POLYNOMIAL(8,139, -22, -72, -151, -43, 185, 23, 20, -19) << endl;
cout << POLYNOMIAL(3,76, -124, 109, 31) + POLYNOMIAL(3,-61, -60, -86, -4) << endl;
cout << POLYNOMIAL(4,-156, -161, 148, 168, -138) + POLYNOMIAL(4,69, 167, 175, -152, -85) << endl;
cout << POLYNOMIAL(7,-147, -126, -68, -37, 1, -66, 156, 132) + POLYNOMIAL(7,174, 142, -190, -148, -185, 29, -22, -62) << endl;
cout << POLYNOMIAL(3,33, -62, 186, -198) + POLYNOMIAL(3,-104, 86, -90, 19) << endl;
cout << POLYNOMIAL(2,-138, -140, -157) + POLYNOMIAL(2,-143, -156, -86) << endl;
cout << POLYNOMIAL(9,200, 81, 130, -133, -40, -33, -118, 59, -71, 134) + POLYNOMIAL(9,160, -149, -14, -59, 132, -112, 119, 120, -105, 103) << endl;
cout << POLYNOMIAL(3,85, 27, 75, 159) + POLYNOMIAL(3,-126, -162, 8, 8) << endl;
cout << POLYNOMIAL(3,118, -180, -86, -71) + POLYNOMIAL(3,-28, 150, 75, 193) << endl;
cout << POLYNOMIAL(2,-175, -110, 137) + POLYNOMIAL(2,148, 200, -8) << endl;
cout << POLYNOMIAL(7,152, 95, 100, -31, 153, 106, 71, 132) + POLYNOMIAL(7,74, 16, -196, 186, -167, -125, -149, -84) << endl;
cout << POLYNOMIAL(8,145, 159, -6, 133, -28, 123, 161, 27, 35) + POLYNOMIAL(8,-189, -57, 53, 95, -157, 91, 118, -102, -167) << endl;
cout << POLYNOMIAL(8,71, -111, 79, 91, -157, 139, 90, 123, 170) + POLYNOMIAL(8,11, 62, -122, 140, -100, -138, -28, -104, -60) << endl;
cout << POLYNOMIAL(7,-62, 39, 99, 3, 186, 75, 122, -28) + POLYNOMIAL(7,-183, -132, -131, -188, 174, -185, -185, -68) << endl;
cout << POLYNOMIAL(2,-187, -81, 69) + POLYNOMIAL(2,93, -193, -76) << endl;
cout << POLYNOMIAL(7,-41, -152, -182, -167, -174, 199, 172, -41) + POLYNOMIAL(7,186, 111, -131, 57, 118, 70, 3, -45) << endl;
cout << POLYNOMIAL(9,5, 164, 191, 106, -125, -143, 94, -176, -122, 66) + POLYNOMIAL(9,-182, 54, 122, -190, 15, 36, -104, -182, -50, -82) << endl;
cout << POLYNOMIAL(6,197, 77, -168, 197, -167, 68, -93) + POLYNOMIAL(6,-157, 23, 118, -194, 18, -39, -129) << endl;
cout << POLYNOMIAL(1,37, 136) + POLYNOMIAL(1,124, -59) << endl;
cout << POLYNOMIAL(6,80, -184, -184, 129, -99, 41, -156) + POLYNOMIAL(6,-25, 181, -99, -148, -174, -190, -82) << endl;
cout << POLYNOMIAL(4,88, -21, 115, 129, -96) + POLYNOMIAL(4,-197, 21, -62, -8, -195) << endl;
cout << POLYNOMIAL(8,74, 109, 124, 181, 49, -6, -132, -146, -126) + POLYNOMIAL(8,71, -163, -58, -76, 190, 71, -139, -146, -95) << endl;
cout << POLYNOMIAL(7,95, -113, -29, -7, -178, 115, -154, -52) + POLYNOMIAL(7,-20, 0, 5, -158, 200, -91, -174, 44) << endl;
cout << POLYNOMIAL(7,-58, -156, 110, 199, 72, -190, -6, -54) + POLYNOMIAL(7,-125, -154, 121, 71, -166, -8, 44, -44) << endl;
cout << POLYNOMIAL(0,-156) + POLYNOMIAL(0,157) << endl;
cout << POLYNOMIAL(0,-149) + POLYNOMIAL(0,-43) << endl;
cout << POLYNOMIAL(4,143, 124, 137, -51, 119) + POLYNOMIAL(4,149, -178, -163, 70, -15) << endl;
cout << POLYNOMIAL(9,95, -89, 134, 111, -141, 24, -73, 125, 53, 160) + POLYNOMIAL(9,75, -75, -35, 145, 100, -44, 72, 22, -113, -181) << endl;
cout << POLYNOMIAL(3,-197, -101, -131, 147) + POLYNOMIAL(3,-32, -43, 113, 148) << endl;
cout << POLYNOMIAL(2,173, -121, -111) + POLYNOMIAL(2,74, -137, -1) << endl;
cout << POLYNOMIAL(3,123, 114, 22, 32) + POLYNOMIAL(3,-21, 107, 146, -6) << endl;
cout << POLYNOMIAL(3,175, 149, 132, -26) + POLYNOMIAL(3,-81, 119, 158, 40) << endl;
cout << POLYNOMIAL(7,-65, 147, -65, 130, 36, -198, -80, -168) + POLYNOMIAL(7,-113, 200, 57, -131, -130, 43, -96, -5) << endl;
cout << POLYNOMIAL(2,-108, 75, -47) + POLYNOMIAL(2,148, -11, -189) << endl;
cout << POLYNOMIAL(1,95, -128) + POLYNOMIAL(1,172, 90) << endl;
cout << POLYNOMIAL(0,-21) + POLYNOMIAL(0,-21) << endl;
cout << POLYNOMIAL(9,-9, 36, -144, 140, 147, 36, 76, -190, 42, 185) + POLYNOMIAL(9,-158, 17, 25, -120, -101, -166, -44, 51, 184, 96) << endl;
cout << POLYNOMIAL(9,-118, -28, 104, 200, 6, 121, -40, 181, -120, -134) + POLYNOMIAL(9,-12, 41, -125, -185, 78, -89, 67, -115, -160, -64) << endl;
cout << POLYNOMIAL(6,-198, 98, 111, 195, 135, -62, -46) + POLYNOMIAL(6,11, -182, 120, -119, 97, 27, 177) << endl;
cout << POLYNOMIAL(4,-193, 50, 66, 107, -77) + POLYNOMIAL(4,-62, 151, 144, -5, 109) << endl;
cout << POLYNOMIAL(7,-193, 169, 118, 76, 192, -162, -142, 192) + POLYNOMIAL(7,97, -180, -57, -162, -13, -8, -32, -172) << endl;
cout << POLYNOMIAL(5,16, 55, 0, -40, -11, -192) + POLYNOMIAL(5,163, 67, -65, 6, -46, 7) << endl;
cout << POLYNOMIAL(7,-80, 131, -102, -51, 197, 14, 55, -150) + POLYNOMIAL(7,68, 132, -114, 93, -113, 115, 4, -168) << endl;
cout << POLYNOMIAL(7,-37, -134, 34, 43, 186, 2, -156, 176) + POLYNOMIAL(7,7, 190, 160, -9, -164, -26, -180, 132) << endl;
cout << POLYNOMIAL(4,97, -52, 100, -139, -83) + POLYNOMIAL(4,133, 182, 176, -99, 160) << endl;
cout << POLYNOMIAL(1,129, 116) + POLYNOMIAL(1,30, 126) << endl;
cout << POLYNOMIAL(6,135, -15, 53, 151, 36, 45, -110) + POLYNOMIAL(6,-195, 162, -135, -171, -125, -162, 130) << endl;
cout << POLYNOMIAL(5,179, 99, -65, 179, -197, 115) + POLYNOMIAL(5,-62, 102, -89, -34, -199, 75) << endl;
cout << POLYNOMIAL(0,37) + POLYNOMIAL(0,197) << endl;
cout << POLYNOMIAL(4,-169, -96, 88, -196, -120) + POLYNOMIAL(4,-145, 10, 69, 137, -45) << endl;
cout << POLYNOMIAL(6,-193, 139, 128, 118, -125, -62, 61) + POLYNOMIAL(6,-198, -146, 17, -28, 91, -21, 155) << endl;
cout << POLYNOMIAL(4,-103, 6, -128, -135, 142) + POLYNOMIAL(4,-124, 133, -79, -8, -154) << endl;
cout << POLYNOMIAL(0,-94) + POLYNOMIAL(0,186) << endl;
cout << POLYNOMIAL(8,-27, -150, 28, -43, 12, 20, -12, -57, -199) + POLYNOMIAL(8,182, 92, -194, 11, 163, -54, -130, 29, -30) << endl;
cout << POLYNOMIAL(8,-181, 171, 104, 190, 16, 154, 152, 91, 74) + POLYNOMIAL(8,118, -131, -133, -96, -3, 129, 135, -175, 55) << endl;
cout << POLYNOMIAL(5,191, 150, -171, 10, -32, -98) + POLYNOMIAL(5,151, -33, 157, 42, 165, -66) << endl;
cout << POLYNOMIAL(5,-57, -81, -15, 82, 87, -127) + POLYNOMIAL(5,-76, -66, -13, 87, -102, -2) << endl;
cout << POLYNOMIAL(7,101, -72, -101, -60, -53, -117, -88, -78) + POLYNOMIAL(7,-107, -21, 110, -125, -147, -124, -104, 187) << endl;
cout << POLYNOMIAL(6,-180, -107, -141, -128, -103, 8, 125) + POLYNOMIAL(6,40, -29, -75, -14, 127, 58, 141) << endl;
cout << POLYNOMIAL(9,138, 120, 77, 57, -7, -81, 57, 19, 19, 75) + POLYNOMIAL(9,-97, -123, -138, 20, 197, 14, 39, 149, -149, -178) << endl;
cout << POLYNOMIAL(8,-25, 134, 166, 156, -63, -114, -177, 47, -195) + POLYNOMIAL(8,122, -74, -127, -65, 56, 67, -91, 138, -112) << endl;
cout << POLYNOMIAL(6,138, 177, 25, -110, 142, -19, -191) + POLYNOMIAL(6,-28, 108, -71, 143, 133, 62, -38) << endl;
cout << POLYNOMIAL(4,71, 127, 150, -3, 153) + POLYNOMIAL(4,142, -124, -185, -173, -108) << endl;
cout << POLYNOMIAL(6,-26, -138, -1, 71, 42, -83, -98) + POLYNOMIAL(6,172, -86, 22, -131, 6, -138, -111) << endl;
cout << POLYNOMIAL(1,-189, 42) + POLYNOMIAL(1,116, -29) << endl;
cout << POLYNOMIAL(1,-179, -53) + POLYNOMIAL(1,-42, 152) << endl;
cout << POLYNOMIAL(6,-132, -108, 76, 105, 135, -102, 1) + POLYNOMIAL(6,-75, -33, -110, -100, 17, 80, -64) << endl;
cout << POLYNOMIAL(1,-190, -134) + POLYNOMIAL(1,-191, -129) << endl;
cout << POLYNOMIAL(9,185, -138, 6, -77, 125, 142, -153, 144, 140, 88) + POLYNOMIAL(9,-65, -133, -184, 56, -76, -99, 44, 104, 28, -112) << endl;
cout << POLYNOMIAL(2,193, -141, -39) + POLYNOMIAL(2,164, -105, -30) << endl;
cout << POLYNOMIAL(5,-31, -16, 133, -24, 33, -51) + POLYNOMIAL(5,37, -164, -175, 35, 0, 37) << endl;
cout << POLYNOMIAL(3,19, 131, -107, 178) + POLYNOMIAL(3,31, 44, 47, 102) << endl;
cout << POLYNOMIAL(6,-91, -85, 144, -157, -85, 181, -65) + POLYNOMIAL(6,19, -144, 195, -3, -199, -149, -79) << endl;
cout << POLYNOMIAL(3,39, 84, -68, -158) + POLYNOMIAL(3,164, -193, 158, -180) << endl;
cout << POLYNOMIAL(8,-188, -34, -179, -59, -112, 11, 107, 102, 37) + POLYNOMIAL(8,-23, -120, 156, 23, -136, 19, 184, 144, -159) << endl;
cout << POLYNOMIAL(0,47) + POLYNOMIAL(0,172) << endl;
cout << POLYNOMIAL(7,29, 68, 91, 20, 160, 95, 200, -98) + POLYNOMIAL(7,137, 65, -24, 148, -34, 4, 128, 146) << endl;
cout << POLYNOMIAL(3,-38, -180, 196, -120) + POLYNOMIAL(3,-29, -95, -88, 172) << endl;
cout << POLYNOMIAL(6,20, -29, -94, -105, 88, 35, 57) + POLYNOMIAL(6,-123, 190, 88, 9, 69, 130, -141) << endl;
cout << POLYNOMIAL(1,195, -177) + POLYNOMIAL(1,59, -198) << endl;
cout << POLYNOMIAL(9,-78, 47, -13, -166, -41, 41, 30, 58, -45, 71) + POLYNOMIAL(9,-26, 112, 9, -65, -124, 94, 192, -49, -129, -122) << endl;
cout << POLYNOMIAL(6,33, 145, 69, 121, 10, 68, 180) + POLYNOMIAL(6,47, 132, -60, 88, -160, 93, 150) << endl;
cout << POLYNOMIAL(7,166, 190, -67, 75, -91, -31, -170, -79) + POLYNOMIAL(7,96, -179, -69, -146, -33, 184, 143, 101) << endl;
cout << POLYNOMIAL(4,-5, 97, -173, 23, -32) + POLYNOMIAL(4,167, -183, -116, -142, 165) << endl;
cout << POLYNOMIAL(6,170, -176, 10, 16, 135, 7, -164) + POLYNOMIAL(6,152, -36, -195, -14, -112, -80, 59) << endl;
cout << POLYNOMIAL(7,-3, -137, 11, 142, 145, -21, -3, -60) + POLYNOMIAL(7,183, -99, 154, 62, 113, -143, -82, 134) << endl;
cout << POLYNOMIAL(7,-144, -181, -101, -168, 23, -176, 113, 158) + POLYNOMIAL(7,196, 148, -12, 9, 124, 158, 106, 68) << endl;
cout << POLYNOMIAL(4,167, 151, 138, 109, -146) + POLYNOMIAL(4,-199, -189, -180, 0, 81) << endl;
cout << POLYNOMIAL(6,-156, -172, -80, -140, -198, -181, 110) + POLYNOMIAL(6,183, -185, 135, 188, -193, -122, 154) << endl;
cout << POLYNOMIAL(7,-4, 66, -55, -78, -105, -162, 147, -64) + POLYNOMIAL(7,-32, -180, 55, 85, 150, 114, -105, -8) << endl;
cout << POLYNOMIAL(1,-131, -18) + POLYNOMIAL(1,-46, -54) << endl;
cout << POLYNOMIAL(2,94, -74, -39) + POLYNOMIAL(2,136, -40, 107) << endl;
cout << POLYNOMIAL(2,-28, -67, 9) + POLYNOMIAL(2,77, -108, -177) << endl;
cout << POLYNOMIAL(2,175, 186, -117) + POLYNOMIAL(2,-176, 37, 127) << endl;
cout << POLYNOMIAL(7,-26, -140, -84, 159, -93, 49, 173, 125) + POLYNOMIAL(7,70, -193, 173, -29, 41, 176, 16, 29) << endl;
cout << POLYNOMIAL(9,27, 185, -99, -110, -91, 175, 139, 68, -164, -162) + POLYNOMIAL(9,-75, 39, 97, -100, 15, 127, -184, -7, 97, -178) << endl;
cout << POLYNOMIAL(5,-98, 1, 194, -99, -92, 169) + POLYNOMIAL(5,172, 68, -122, 96, -65, 69) << endl;
cout << POLYNOMIAL(1,-76, -140) + POLYNOMIAL(1,-169, 160) << endl;
cout << POLYNOMIAL(7,69, -99, 61, 92, -17, 103, 170, -62) + POLYNOMIAL(7,-51, -18, -41, 87, 104, -57, -92, 57) << endl;
cout << POLYNOMIAL(9,-151, -81, 23, -142, -77, -55, -149, 111, -130, 131) + POLYNOMIAL(9,188, 149, 45, -128, 41, 147, -110, -24, 34, -12) << endl;
cout << POLYNOMIAL(8,-48, -97, -6, -44, -21, -10, 151, 4, 184) + POLYNOMIAL(8,130, 45, 41, 26, -31, 56, 71, -78, -157) << endl;
cout << POLYNOMIAL(1,-74, -174) + POLYNOMIAL(1,-191, -88) << endl;
cout << POLYNOMIAL(8,190, -119, -200, -60, -188, -108, -78, -8, 112) + POLYNOMIAL(8,-161, 167, 12, -121, -34, 157, 175, 47, -83) << endl;
cout << POLYNOMIAL(8,-162, 165, -98, -175, -23, 104, -27, -18, 178) + POLYNOMIAL(8,126, 57, 119, -30, -73, -117, -99, -84, -31) << endl;
cout << POLYNOMIAL(4,141, 136, -153, 63, -51) + POLYNOMIAL(4,62, -67, 183, 122, -117) << endl;
cout << POLYNOMIAL(9,25, -25, 196, -81, -64, -99, -152, -101, 107, -157) + POLYNOMIAL(9,12, 57, 181, 55, -119, -152, 109, -197, -30, -23) << endl;
cout << POLYNOMIAL(9,75, -199, -125, -68, 103, -83, 49, 138, 12, 168) + POLYNOMIAL(9,-36, 63, -95, -143, 166, 164, 139, -66, 93, 66) << endl;
cout << POLYNOMIAL(8,128, -75, -162, 80, 65, 97, -97, -68, 64) + POLYNOMIAL(8,-124, -135, -153, -89, -115, 109, 161, -106, 13) << endl;
cout << POLYNOMIAL(6,-180, 65, -183, 192, -181, -20, -200) + POLYNOMIAL(6,-19, -181, -33, -128, 179, -89, 29) << endl;
cout << POLYNOMIAL(6,71, 148, -73, 9, -36, -168, 170) + POLYNOMIAL(6,-159, 7, -67, -146, -181, 77, 91) << endl;
cout << POLYNOMIAL(3,121, 157, -41, 119) + POLYNOMIAL(3,92, 69, -174, 69) << endl;
cout << POLYNOMIAL(9,-149, -160, 57, 136, -178, 165, -51, -5, 86, -72) + POLYNOMIAL(9,29, 189, 25, 9, 92, -114, -15, -138, -82, -5) << endl;
cout << POLYNOMIAL(1,-175, -53) + POLYNOMIAL(1,153, 64) << endl;
cout << POLYNOMIAL(1,-73, 124) + POLYNOMIAL(1,199, -172) << endl;
cout << POLYNOMIAL(5,145, -78, -99, 85, -106, -102) + POLYNOMIAL(5,73, 172, -4, 146, -97, 65) << endl;
cout << POLYNOMIAL(5,9, 97, 149, -198, -113, -74) + POLYNOMIAL(5,51, -8, 165, -152, -105, 76) << endl;
cout << POLYNOMIAL(1,-167, 57) + POLYNOMIAL(1,-139, 153) << endl;
cout << POLYNOMIAL(3,-197, -109, 189, -61) + POLYNOMIAL(3,-191, 80, -68, -106) << endl;
cout << POLYNOMIAL(0,-158) + POLYNOMIAL(0,-133) << endl;
cout << POLYNOMIAL(0,-8) + POLYNOMIAL(0,105) << endl;
cout << POLYNOMIAL(8,177, -91, 128, 48, 70, 22, 129, 140, -158) + POLYNOMIAL(8,77, 15, 21, -16, -105, 181, 140, 103, 195) << endl;
cout << POLYNOMIAL(8,-198, 166, 162, 25, -160, -42, -88, -96, 18) + POLYNOMIAL(8,183, -153, -137, -200, -156, -30, 14, -53, 62) << endl;
cout << POLYNOMIAL(3,-132, -89, 21, -142) + POLYNOMIAL(3,43, -197, 18, -4) << endl;
cout << POLYNOMIAL(5,-94, 3, -97, -133, 88, -46) + POLYNOMIAL(5,-2, -28, 76, 12, 6, -17) << endl;
cout << POLYNOMIAL(8,-8, -79, -115, -41, -56, 89, 175, 197, 103) + POLYNOMIAL(8,-19, 51, -197, -28, -96, -31, 58, 22, 79) << endl;
cout << POLYNOMIAL(4,159, -82, -156, 199, -172) + POLYNOMIAL(4,-127, 84, 81, -98, 43) << endl;
cout << POLYNOMIAL(8,-43, -80, 41, 132, -67, 128, -157, -60, 62) + POLYNOMIAL(8,107, -73, 171, -13, 21, 181, 136, 23, 58) << endl;
cout << POLYNOMIAL(3,-95, 176, 159, 79) + POLYNOMIAL(3,-178, 31, -197, -69) << endl;
cout << POLYNOMIAL(7,164, 158, -56, -128, -72, -6, 18, 56) + POLYNOMIAL(7,16, 3, -32, 163, -152, 145, 181, -107) << endl;
cout << POLYNOMIAL(5,-106, -116, 188, 75, -140, -73) + POLYNOMIAL(5,-12, -173, -32, 2, 129, 98) << endl;
cout << POLYNOMIAL(3,-144, -48, -92, -164) + POLYNOMIAL(3,193, -76, -130, 61) << endl;
cout << POLYNOMIAL(3,-173, -19, -174, -17) + POLYNOMIAL(3,145, -185, 49, -149) << endl;
cout << POLYNOMIAL(3,69, -74, -18, 117) + POLYNOMIAL(3,-58, 44, -7, 119) << endl;
cout << POLYNOMIAL(9,116, 118, 0, -141, 27, 174, -2, 129, 127, 143) + POLYNOMIAL(9,-112, -178, 19, -196, -28, 120, -165, 182, 67, -123) << endl;
cout << POLYNOMIAL(1,74, -134) + POLYNOMIAL(1,127, -108) << endl;
cout << POLYNOMIAL(7,107, 179, -42, -17, -114, -75, 3, 81) + POLYNOMIAL(7,44, -65, -194, 24, 162, 19, 131, 165) << endl;
cout << POLYNOMIAL(1,34, -188) + POLYNOMIAL(1,-73, 41) << endl;
cout << POLYNOMIAL(2,-179, -190, 1) + POLYNOMIAL(2,-49, -193, -64) << endl;
cout << POLYNOMIAL(6,-200, 29, 7, -69, 158, 95, 27) + POLYNOMIAL(6,152, -155, 150, 31, 15, 83, 44) << endl;
cout << POLYNOMIAL(7,187, 33, 149, 180, -3, 129, -72, 25) + POLYNOMIAL(7,33, -28, -47, 118, -187, 58, -72, -46) << endl;
cout << POLYNOMIAL(5,96, -193, 73, -197, 23, -119) + POLYNOMIAL(5,117, -158, -74, 43, 149, 178) << endl;
cout << POLYNOMIAL(3,-200, -18, 2, -38) + POLYNOMIAL(3,-113, 162, -28, -33) << endl;
cout << POLYNOMIAL(9,155, 98, 108, -118, 0, -49, -195, -100, 129, -128) + POLYNOMIAL(9,-100, 23, -175, -53, -77, 70, -37, -175, 15, 185) << endl;
cout << POLYNOMIAL(5,-84, -100, -53, 133, 67, -34) + POLYNOMIAL(5,65, -138, 154, -27, -128, -87) << endl;
cout << POLYNOMIAL(3,154, -6, 97, -53) + POLYNOMIAL(3,-4, -18, 73, -126) << endl;
cout << POLYNOMIAL(9,-111, 198, 97, -143, -190, -148, -1, -39, -153, 84) + POLYNOMIAL(9,-193, -167, 114, -189, 28, -39, -134, -51, -19, 116) << endl;
cout << POLYNOMIAL(5,-74, -120, 66, 166, 123, 63) + POLYNOMIAL(5,-27, 22, -141, 178, -8, -167) << endl;
cout << POLYNOMIAL(6,-3, 178, -112, 69, -91, 103, 63) + POLYNOMIAL(6,170, -4, -13, -1, -28, -81, 107) << endl;
cout << POLYNOMIAL(1,50, -77) + POLYNOMIAL(1,-100, 135) << endl;
cout << POLYNOMIAL(6,-28, 195, -186, 78, -114, 199, 42) + POLYNOMIAL(6,-76, 189, -152, -48, -168, -57, -7) << endl;
cout << POLYNOMIAL(0,87) + POLYNOMIAL(0,85) << endl;
cout << POLYNOMIAL(4,160, -159, -127, -178, 15) + POLYNOMIAL(4,49, 172, 129, 155, -199) << endl;
cout << POLYNOMIAL(7,-16, -1, -136, 64, -62, 179, 92, -93) + POLYNOMIAL(7,75, -61, -186, -6, 197, -39, -45, -29) << endl;
cout << POLYNOMIAL(6,55, 181, 147, 125, -116, 45, 18) + POLYNOMIAL(6,126, 13, 7, -62, -141, 38, -92) << endl;
cout << POLYNOMIAL(8,24, 182, 114, 192, -63, -186, 54, 73, 84) + POLYNOMIAL(8,103, 37, 180, -190, -104, -153, -85, -179, 50) << endl;
cout << POLYNOMIAL(1,101, 157) + POLYNOMIAL(1,53, -20) << endl;
cout << POLYNOMIAL(7,-45, 135, -127, 159, -119, 110, 25, -143) + POLYNOMIAL(7,-53, -189, 123, -170, -198, -23, -131, 6) << endl;
cout << POLYNOMIAL(7,-9, -154, -18, -150, -188, -114, -11, -119) + POLYNOMIAL(7,-137, 25, 2, 120, -43, 76, -72, -48) << endl;
cout << POLYNOMIAL(5,-9, -166, 59, 143, 190, 37) + POLYNOMIAL(5,-154, -50, 79, 189, -150, -62) << endl;
cout << POLYNOMIAL(1,114, -2) + POLYNOMIAL(1,-41, -58) << endl;
cout << POLYNOMIAL(3,-195, -86, 40, 104) + POLYNOMIAL(3,37, -66, 100, -103) << endl;
cout << POLYNOMIAL(6,-38, -167, 105, -106, -120, -14, -56) + POLYNOMIAL(6,-97, -151, 137, -10, -200, -98, 119) << endl;
cout << POLYNOMIAL(7,66, 86, 126, 145, 47, -183, 137, 152) + POLYNOMIAL(7,199, 179, 67, 28, -2, -160, 134, 14) << endl;
cout << POLYNOMIAL(3,-198, 163, -181, -75) + POLYNOMIAL(3,-139, -164, -145, 43) << endl;
cout << POLYNOMIAL(7,113, -36, -4, 128, 23, 82, 82, 64) + POLYNOMIAL(7,80, -9, -46, 42, -118, -113, 7, 30) << endl;
cout << POLYNOMIAL(9,150, -36, 138, 32, -7, 186, 52, 119, -174, -143) + POLYNOMIAL(9,-12, 140, -122, 147, 174, -184, 165, 186, 42, -190) << endl;
cout << POLYNOMIAL(9,-111, 91, -152, 82, -43, -84, 44, -25, 174, -55) + POLYNOMIAL(9,198, 157, 166, -144, -166, 40, 37, 53, 17, -60) << endl;
cout << POLYNOMIAL(9,117, -195, -14, 16, -123, -179, -39, -187, -4, -96) + POLYNOMIAL(9,-175, -142, -169, -105, 60, -56, 191, 10, 145, 98) << endl;
cout << POLYNOMIAL(6,-148, -61, -145, 39, 54, 152, -9) + POLYNOMIAL(6,-3, 194, 142, -60, -2, 174, 189) << endl;
cout << POLYNOMIAL(6,111, 140, 99, -170, -199, 146, 30) + POLYNOMIAL(6,-27, -153, 38, -96, -116, 194, -189) << endl;
cout << POLYNOMIAL(6,-91, -137, 192, 194, 160, 169, 9) + POLYNOMIAL(6,-31, 11, 148, 165, 30, -135, -58) << endl;
cout << POLYNOMIAL(0,-61) + POLYNOMIAL(0,-135) << endl;
cout << POLYNOMIAL(6,-67, -54, -40, -175, 23, 32, -174) + POLYNOMIAL(6,-14, -172, -113, -70, -83, -130, 68) << endl;
cout << POLYNOMIAL(2,102, 131, -192) + POLYNOMIAL(2,-25, -98, -117) << endl;
cout << POLYNOMIAL(5,48, 13, 124, 117, 128, 178) + POLYNOMIAL(5,180, 189, 83, 187, -97, 110) << endl;
cout << POLYNOMIAL(5,-141, -198, 96, -149, 17, 72) + POLYNOMIAL(5,131, 68, 50, -64, 56, 188) << endl;
cout << POLYNOMIAL(6,119, -62, 72, -4, 90, 40, 74) + POLYNOMIAL(6,119, 164, -135, -124, -54, 51, 190) << endl;
cout << POLYNOMIAL(4,71, -167, -29, -85, 170) + POLYNOMIAL(4,188, 168, 145, -199, 16) << endl;
cout << POLYNOMIAL(7,25, 103, 69, 199, 117, 73, 89, 163) + POLYNOMIAL(7,-61, 17, -25, 194, -165, -113, 55, -19) << endl;
cout << POLYNOMIAL(9,-10, -133, 50, 92, -141, -68, 81, 114, 189, -14) + POLYNOMIAL(9,43, 108, 189, 146, -148, -107, 48, -191, -120, -197) << endl;
cout << POLYNOMIAL(0,36) + POLYNOMIAL(0,-181) << endl;
cout << POLYNOMIAL(4,-40, -87, -158, -55, -175) + POLYNOMIAL(4,31, 70, -179, -176, -56) << endl;
cout << POLYNOMIAL(4,-51, 24, -199, 192, -49) + POLYNOMIAL(4,-12, 126, -54, -200, 3) << endl;
cout << POLYNOMIAL(5,-169, 176, -171, 100, -90, -139) + POLYNOMIAL(5,-193, 0, 38, 156, -79, -67) << endl;
cout << POLYNOMIAL(4,89, 129, 66, -77, 161) + POLYNOMIAL(4,-57, -14, -17, -23, 80) << endl;
cout << POLYNOMIAL(7,-21, 24, 196, 23, 111, 191, -195, 169) + POLYNOMIAL(7,159, 101, -111, -81, 199, 13, 177, -9) << endl;
cout << POLYNOMIAL(5,-35, -46, -124, -61, -62, 75) + POLYNOMIAL(5,37, 178, -146, 100, 191, -122) << endl;
cout << POLYNOMIAL(9,-15, 144, 126, -118, -8, 19, -16, -81, -4, -126) + POLYNOMIAL(9,-55, -193, -134, 115, -130, 1, 55, 118, -165, -108) << endl;
cout << POLYNOMIAL(9,-129, 23, -135, -195, -136, -141, 189, 85, 131, 34) + POLYNOMIAL(9,-120, 67, 32, -63, -135, 90, 179, 16, 194, 181) << endl;
cout << POLYNOMIAL(3,190, 189, -73, 44) + POLYNOMIAL(3,23, -48, 5, 103) << endl;
cout << POLYNOMIAL(1,-116, 76) + POLYNOMIAL(1,196, 117) << endl;
cout << POLYNOMIAL(3,-61, 108, -136, -49) + POLYNOMIAL(3,64, -30, 105, -91) << endl;
cout << POLYNOMIAL(6,23, -40, 109, 167, -96, 157, 149) + POLYNOMIAL(6,-85, 174, -195, 27, 104, -63, -149) << endl;
cout << POLYNOMIAL(7,17, 160, 73, -49, 172, 170, 148, -55) + POLYNOMIAL(7,-78, -131, 29, -46, -178, -73, -149, -199) << endl;
cout << POLYNOMIAL(9,158, -188, 97, -3, 6, 82, -158, 193, -23, -118) + POLYNOMIAL(9,52, -143, 62, -142, 53, -7, 41, -62, -81, -196) << endl;
cout << POLYNOMIAL(8,149, -77, -122, -154, -27, 185, -2, 90, 156) + POLYNOMIAL(8,10, 96, 142, 11, -164, 183, -62, 48, -22) << endl;
cout << POLYNOMIAL(9,170, 129, 68, -131, -181, 139, -185, -46, -89, -40) + POLYNOMIAL(9,91, 31, 154, -164, -4, -173, 149, -182, 194, -76) << endl;
cout << POLYNOMIAL(0,-115) + POLYNOMIAL(0,-191) << endl;
cout << POLYNOMIAL(4,-179, -177, 99, -82, -53) + POLYNOMIAL(4,-52, 157, 23, 182, -129) << endl;
cout << POLYNOMIAL(7,8, -38, 82, 199, -64, 7, -156, -154) + POLYNOMIAL(7,4, -13, 108, 50, 21, -8, 108, -67) << endl;
cout << POLYNOMIAL(8,-193, 61, -138, -150, -148, -151, 38, 196, -58) + POLYNOMIAL(8,-67, 156, 128, -194, -117, 128, 29, -193, 63) << endl;
cout << POLYNOMIAL(9,-73, 38, -2, 35, -136, -12, -152, 183, 131, -72) + POLYNOMIAL(9,-44, -133, 35, -38, -7, 60, 126, -66, -42, 184) << endl;
cout << POLYNOMIAL(9,-62, -156, 62, 159, 109, 101, -65, -146, 14, 2) + POLYNOMIAL(9,-97, 113, 155, -28, -138, 49, 105, -124, 184, 68) << endl;
cout << POLYNOMIAL(9,136, -83, 81, -146, -74, 89, -31, -168, -32, -23) + POLYNOMIAL(9,-137, -72, 157, 173, -150, -24, 148, 161, -76, 178) << endl;
cout << POLYNOMIAL(4,148, -159, -106, 36, -85) + POLYNOMIAL(4,67, -127, 14, -198, -54) << endl;
cout << POLYNOMIAL(9,-178, -64, -10, 189, 3, 128, -114, 161, 149, 30) + POLYNOMIAL(9,-11, 165, 90, -60, 56, -3, 140, 182, -160, -72) << endl;
cout << POLYNOMIAL(1,-116, -128) + POLYNOMIAL(1,22, -45) << endl;
cout << POLYNOMIAL(7,11, 136, -53, -46, -31, 19, 5, -86) + POLYNOMIAL(7,-141, 180, -57, -83, -187, -15, 120, 70) << endl;
cout << POLYNOMIAL(7,45, 174, 105, -68, -185, 50, 47, 61) + POLYNOMIAL(7,-132, -83, 44, -80, 58, -139, 160, -167) << endl;
cout << POLYNOMIAL(8,-7, -16, -41, -164, -67, -133, -56, -184, 169) + POLYNOMIAL(8,-163, 198, 198, -128, 145, -69, -111, -105, -97) << endl;
cout << POLYNOMIAL(4,144, -74, -136, -184, -147) + POLYNOMIAL(4,181, -141, 93, -117, 159) << endl;
cout << POLYNOMIAL(2,185, 78, -90) + POLYNOMIAL(2,-62, 4, -198) << endl;
cout << POLYNOMIAL(0,114) + POLYNOMIAL(0,77) << endl;
cout << POLYNOMIAL(4,-75, -77, 164, -112, 20) + POLYNOMIAL(4,-198, -181, -118, 151, 85) << endl;
cout << POLYNOMIAL(0,-185) + POLYNOMIAL(0,-16) << endl;
cout << POLYNOMIAL(4,-63, 25, 170, 177, -174) + POLYNOMIAL(4,-199, 106, -180, -190, 56) << endl;
cout << POLYNOMIAL(6,-156, -146, -146, -10, -1, 174, -97) + POLYNOMIAL(6,51, 9, 195, -57, -1, 102, -64) << endl;
cout << POLYNOMIAL(1,-175, 161) + POLYNOMIAL(1,-188, -169) << endl;
cout << POLYNOMIAL(1,-37, -138) + POLYNOMIAL(1,38, 187) << endl;
cout << POLYNOMIAL(3,-99, 126, 108, 74) + POLYNOMIAL(3,-27, -158, 145, 160) << endl;
cout << POLYNOMIAL(2,-94, -125, -176) + POLYNOMIAL(2,-196, 143, 30) << endl;
cout << POLYNOMIAL(5,-186, 84, 35, 93, -77, 97) + POLYNOMIAL(5,-75, 0, -83, 17, 86, 87) << endl;
cout << POLYNOMIAL(0,42) + POLYNOMIAL(0,125) << endl;
cout << POLYNOMIAL(4,200, 172, 110, -160, 186) + POLYNOMIAL(4,103, -92, -77, 63, -88) << endl;
cout << POLYNOMIAL(5,57, -111, -195, -158, 93, 53) + POLYNOMIAL(5,-195, 94, 176, -121, -104, 70) << endl;
cout << POLYNOMIAL(1,-56, 161) + POLYNOMIAL(1,23, 161) << endl;
cout << POLYNOMIAL(6,90, -185, 96, -175, 189, -145, -138) + POLYNOMIAL(6,-74, -180, 170, -179, 191, -3, 116) << endl;
cout << POLYNOMIAL(2,-9, 64, -187) + POLYNOMIAL(2,-198, 160, -96) << endl;
cout << POLYNOMIAL(7,22, 48, 75, 67, 6, -123, -34, -38) + POLYNOMIAL(7,-101, -38, -56, 68, 141, 5, -55, -148) << endl;
cout << POLYNOMIAL(8,-148, 170, -54, -125, -80, 161, -169, -178, -87) + POLYNOMIAL(8,-112, 7, 159, -69, 168, 144, 112, -5, -133) << endl;
cout << POLYNOMIAL(0,-197) + POLYNOMIAL(0,85) << endl;
cout << POLYNOMIAL(8,187, -17, -155, 6, -200, -28, -86, 161, -92) + POLYNOMIAL(8,-88, -167, -40, -191, -8, -43, -85, 153, -54) << endl;
cout << POLYNOMIAL(2,-200, 10, -199) + POLYNOMIAL(2,-46, 58, 91) << endl;
cout << POLYNOMIAL(0,59) + POLYNOMIAL(0,117) << endl;
cout << POLYNOMIAL(5,181, -61, 61, 156, 82, 118) + POLYNOMIAL(5,111, -138, 70, 150, -189, -126) << endl;
cout << POLYNOMIAL(3,-156, 187, 181, 83) + POLYNOMIAL(3,67, -79, 33, 44) << endl;
cout << POLYNOMIAL(3,-145, -118, -192, 147) + POLYNOMIAL(3,126, 8, 71, -21) << endl;
cout << POLYNOMIAL(1,-194, -91) + POLYNOMIAL(1,-157, -115) << endl;
cout << POLYNOMIAL(9,-39, 130, -178, -81, -9, -140, 21, 54, 122, 174) + POLYNOMIAL(9,40, 148, -141, 5, 58, 45, -65, 177, -104, 88) << endl;
cout << POLYNOMIAL(7,173, 133, -27, -113, 131, 127, -131, -23) + POLYNOMIAL(7,-119, 152, -152, -108, -123, 129, 32, 90) << endl;
cout << POLYNOMIAL(5,17, 74, -124, 61, -155, 196) + POLYNOMIAL(5,-199, -155, -116, 45, -144, -133) << endl;
cout << POLYNOMIAL(0,-119) + POLYNOMIAL(0,-147) << endl;
cout << POLYNOMIAL(2,132, 132, -18) + POLYNOMIAL(2,177, -187, -78) << endl;
cout << POLYNOMIAL(3,-86, 108, -122, 68) + POLYNOMIAL(3,119, 135, 164, 198) << endl;
cout << POLYNOMIAL(8,123, -96, -102, 99, 196, 33, -3, 169, 78) + POLYNOMIAL(8,-132, 92, -197, -88, 106, 110, -144, 146, -162) << endl;
cout << POLYNOMIAL(9,145, 87, -116, 78, -2, -19, -46, -184, -38, -83) + POLYNOMIAL(9,144, -153, -188, -126, -147, -5, -179, 174, 54, 189) << endl;
cout << POLYNOMIAL(3,51, 68, 58, 79) + POLYNOMIAL(3,-118, 98, -196, -116) << endl;
cout << POLYNOMIAL(7,16, -160, 165, -109, 146, -148, -34, 24) + POLYNOMIAL(7,197, 195, -197, -126, -79, 153, 75, -186) << endl;
cout << POLYNOMIAL(5,-192, 126, -29, -157, -127, 57) + POLYNOMIAL(5,118, -133, -119, 43, 97, -150) << endl;
cout << POLYNOMIAL(8,84, -98, -188, 152, 20, -36, -173, 79, -162) + POLYNOMIAL(8,-116, -135, -41, 29, 114, -189, 66, 197, -77) << endl;
cout << POLYNOMIAL(8,132, -91, 122, -128, -117, 149, 186, -177, -148) + POLYNOMIAL(8,-54, -194, -148, 81, -140, -4, 131, 159, -122) << endl;
cout << POLYNOMIAL(1,151, 102) + POLYNOMIAL(1,-107, 6) << endl;
cout << POLYNOMIAL(8,-7, -116, -174, -123, -168, 166, -7, 144, 134) + POLYNOMIAL(8,38, 153, 99, 84, -93, 132, 173, 67, -194) << endl;
cout << POLYNOMIAL(7,155, -139, -167, 46, -155, 29, -27, 42) + POLYNOMIAL(7,-77, 95, 200, -162, -176, -194, -35, -109) << endl;
cout << POLYNOMIAL(4,-41, -27, -179, 84, 114) + POLYNOMIAL(4,-62, -28, 147, 98, -99) << endl;
cout << POLYNOMIAL(9,151, -151, 103, 64, -107, -2, -170, 138, -47, -47) + POLYNOMIAL(9,75, 124, 140, -50, -86, -167, -52, 78, 13, 63) << endl;
cout << POLYNOMIAL(7,-35, 94, 181, 136, 101, -55, 114, 76) + POLYNOMIAL(7,-181, -168, 147, 79, 129, 170, -181, 65) << endl;
cout << POLYNOMIAL(1,128, 170) + POLYNOMIAL(1,-55, 15) << endl;
return 0;
}