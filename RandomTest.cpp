#include <iostream>

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
        int* newArr = new int[newRank];

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

int main () {

cout << POLYNOMIAL(1, 9, 1) / POLYNOMIAL(2, 2, -7, -8) << endl;
cout << POLYNOMIAL(1, 9, 1) % POLYNOMIAL(2, 2, -7, -8) << endl;
cout << POLYNOMIAL(3, 0, 9, 10, 7) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, 0, 9, 10, 7) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(2, 9, -10, 1) / POLYNOMIAL(3, -4, 0, 3, 4) << endl;
cout << POLYNOMIAL(2, 9, -10, 1) % POLYNOMIAL(3, -4, 0, 3, 4) << endl;
cout << POLYNOMIAL(2, -3, 0, 4) / POLYNOMIAL(3, 7, 1, -1, -8) << endl;
cout << POLYNOMIAL(2, -3, 0, 4) % POLYNOMIAL(3, 7, 1, -1, -8) << endl;
cout << POLYNOMIAL(3, 7, 10, 1, -5) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(3, 7, 10, 1, -5) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(2, -10, -7, 5) / POLYNOMIAL(3, 5, -5, 6, -8) << endl;
cout << POLYNOMIAL(2, -10, -7, 5) % POLYNOMIAL(3, 5, -5, 6, -8) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, 2, -7, -1, -1) / POLYNOMIAL(1, 3, 0) << endl;
cout << POLYNOMIAL(3, 2, -7, -1, -1) % POLYNOMIAL(1, 3, 0) << endl;
cout << POLYNOMIAL(2, 1, 1, 1) / POLYNOMIAL(1, -3, -7) << endl;
cout << POLYNOMIAL(2, 1, 1, 1) % POLYNOMIAL(1, -3, -7) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(2, 7, 3, 8) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(2, 7, 3, 8) << endl;
cout << POLYNOMIAL(2, 1, 10, 9) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(2, 1, 10, 9) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(1, -8, -3) / POLYNOMIAL(1, 6, 9) << endl;
cout << POLYNOMIAL(1, -8, -3) % POLYNOMIAL(1, 6, 9) << endl;
cout << POLYNOMIAL(2, 7, 9, -9) / POLYNOMIAL(3, -2, -1, -2, -10) << endl;
cout << POLYNOMIAL(2, 7, 9, -9) % POLYNOMIAL(3, -2, -1, -2, -10) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(1, -7, 2) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(1, -7, 2) << endl;
cout << POLYNOMIAL(3, -3, -4, 7, -6) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -3, -4, 7, -6) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -10, 10, 6, -1) / POLYNOMIAL(3, 7, -5, -8, -7) << endl;
cout << POLYNOMIAL(3, -10, 10, 6, -1) % POLYNOMIAL(3, 7, -5, -8, -7) << endl;
cout << POLYNOMIAL(2, 8, 6, 2) / POLYNOMIAL(3, 1, -5, -1, 0) << endl;
cout << POLYNOMIAL(2, 8, 6, 2) % POLYNOMIAL(3, 1, -5, -1, 0) << endl;
cout << POLYNOMIAL(1, -2, 8) / POLYNOMIAL(2, 8, 9, 2) << endl;
cout << POLYNOMIAL(1, -2, 8) % POLYNOMIAL(2, 8, 9, 2) << endl;
cout << POLYNOMIAL(1, -9, -6) / POLYNOMIAL(3, -3, -6, 3, 8) << endl;
cout << POLYNOMIAL(1, -9, -6) % POLYNOMIAL(3, -3, -6, 3, 8) << endl;
cout << POLYNOMIAL(1, 8, 3) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(1, 8, 3) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(3, -8, 4, -6, 9) / POLYNOMIAL(1, -5, 2) << endl;
cout << POLYNOMIAL(3, -8, 4, -6, 9) % POLYNOMIAL(1, -5, 2) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(1, -4, 2) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(1, -4, 2) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(3, -4, -6, -2, 4) / POLYNOMIAL(2, -2, -7, 10) << endl;
cout << POLYNOMIAL(3, -4, -6, -2, 4) % POLYNOMIAL(2, -2, -7, 10) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(3, 1, -8, -5, -8) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(3, 1, -8, -5, -8) << endl;
cout << POLYNOMIAL(2, -2, -2, -2) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(2, -2, -2, -2) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(2, 7, -8, 10) / POLYNOMIAL(2, -6, 1, 10) << endl;
cout << POLYNOMIAL(2, 7, -8, 10) % POLYNOMIAL(2, -6, 1, 10) << endl;
cout << POLYNOMIAL(3, -7, -1, -2, -8) / POLYNOMIAL(2, 6, -10, 5) << endl;
cout << POLYNOMIAL(3, -7, -1, -2, -8) % POLYNOMIAL(2, 6, -10, 5) << endl;
cout << POLYNOMIAL(1, -3, -2) / POLYNOMIAL(3, -2, 10, -4, 9) << endl;
cout << POLYNOMIAL(1, -3, -2) % POLYNOMIAL(3, -2, 10, -4, 9) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(3, 7, 8, 6, -3) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(3, 7, 8, 6, -3) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(2, 4, -6, -7) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(2, 4, -6, -7) << endl;
cout << POLYNOMIAL(3, -6, -4, -3, -9) / POLYNOMIAL(2, 7, 5, -6) << endl;
cout << POLYNOMIAL(3, -6, -4, -3, -9) % POLYNOMIAL(2, 7, 5, -6) << endl;
cout << POLYNOMIAL(2, -6, -3, -1) / POLYNOMIAL(1, 3, 8) << endl;
cout << POLYNOMIAL(2, -6, -3, -1) % POLYNOMIAL(1, 3, 8) << endl;
cout << POLYNOMIAL(2, -8, -2, -2) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(2, -8, -2, -2) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(3, 9, 2, -2, 3) / POLYNOMIAL(2, 1, 3, 7) << endl;
cout << POLYNOMIAL(3, 9, 2, -2, 3) % POLYNOMIAL(2, 1, 3, 7) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(1, -9, -1) / POLYNOMIAL(1, 4, -8) << endl;
cout << POLYNOMIAL(1, -9, -1) % POLYNOMIAL(1, 4, -8) << endl;
cout << POLYNOMIAL(3, 8, 8, 4, -8) / POLYNOMIAL(1, 2, 2) << endl;
cout << POLYNOMIAL(3, 8, 8, 4, -8) % POLYNOMIAL(1, 2, 2) << endl;
cout << POLYNOMIAL(1, -4, 7) / POLYNOMIAL(2, -1, 2, 8) << endl;
cout << POLYNOMIAL(1, -4, 7) % POLYNOMIAL(2, -1, 2, 8) << endl;
cout << POLYNOMIAL(3, 3, 5, 10, 7) / POLYNOMIAL(1, 3, -10) << endl;
cout << POLYNOMIAL(3, 3, 5, 10, 7) % POLYNOMIAL(1, 3, -10) << endl;
cout << POLYNOMIAL(3, -2, 4, -3, 4) / POLYNOMIAL(2, -3, 5, -10) << endl;
cout << POLYNOMIAL(3, -2, 4, -3, 4) % POLYNOMIAL(2, -3, 5, -10) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(3, 5, -5, 7, 8) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(3, 5, -5, 7, 8) << endl;
cout << POLYNOMIAL(3, -6, -5, 4, 3) / POLYNOMIAL(3, -1, -10, 2, -2) << endl;
cout << POLYNOMIAL(3, -6, -5, 4, 3) % POLYNOMIAL(3, -1, -10, 2, -2) << endl;
cout << POLYNOMIAL(1, 0, -1) / POLYNOMIAL(3, -10, 0, 1, -6) << endl;
cout << POLYNOMIAL(1, 0, -1) % POLYNOMIAL(3, -10, 0, 1, -6) << endl;
cout << POLYNOMIAL(1, -9, -7) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(1, -9, -7) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(2, -1, 10, 3) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(2, -1, 10, 3) << endl;
cout << POLYNOMIAL(2, -5, 7, -3) / POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(2, -5, 7, -3) % POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(1, 5, 3) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(1, 5, 3) << endl;
cout << POLYNOMIAL(1, -2, -3) / POLYNOMIAL(3, -3, -4, -8, 7) << endl;
cout << POLYNOMIAL(1, -2, -3) % POLYNOMIAL(3, -3, -4, -8, 7) << endl;
cout << POLYNOMIAL(3, -6, -6, 1, 3) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -6, -6, 1, 3) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -3, 2, 3, -1) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(3, -3, 2, 3, -1) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(3, 2, 6, 4, 1) / POLYNOMIAL(2, 4, 9, -10) << endl;
cout << POLYNOMIAL(3, 2, 6, 4, 1) % POLYNOMIAL(2, 4, 9, -10) << endl;
cout << POLYNOMIAL(2, -8, 9, -8) / POLYNOMIAL(3, 6, 4, -1, -8) << endl;
cout << POLYNOMIAL(2, -8, 9, -8) % POLYNOMIAL(3, 6, 4, -1, -8) << endl;
cout << POLYNOMIAL(1, 10, -6) / POLYNOMIAL(2, 5, -6, 3) << endl;
cout << POLYNOMIAL(1, 10, -6) % POLYNOMIAL(2, 5, -6, 3) << endl;
cout << POLYNOMIAL(3, 4, 9, -4, 5) / POLYNOMIAL(3, -8, 4, -3, 4) << endl;
cout << POLYNOMIAL(3, 4, 9, -4, 5) % POLYNOMIAL(3, -8, 4, -3, 4) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(1, 7, 2) / POLYNOMIAL(1, -8, -7) << endl;
cout << POLYNOMIAL(1, 7, 2) % POLYNOMIAL(1, -8, -7) << endl;
cout << POLYNOMIAL(1, -6, 2) / POLYNOMIAL(3, 9, 4, 4, 9) << endl;
cout << POLYNOMIAL(1, -6, 2) % POLYNOMIAL(3, 9, 4, 4, 9) << endl;
cout << POLYNOMIAL(2, -3, 1, 7) / POLYNOMIAL(3, 6, 1, 0, -10) << endl;
cout << POLYNOMIAL(2, -3, 1, 7) % POLYNOMIAL(3, 6, 1, 0, -10) << endl;
cout << POLYNOMIAL(1, -3, -4) / POLYNOMIAL(3, 0, 9, 5, 8) << endl;
cout << POLYNOMIAL(1, -3, -4) % POLYNOMIAL(3, 0, 9, 5, 8) << endl;
cout << POLYNOMIAL(3, -8, 0, 9, -5) / POLYNOMIAL(1, -4, 9) << endl;
cout << POLYNOMIAL(3, -8, 0, 9, -5) % POLYNOMIAL(1, -4, 9) << endl;
cout << POLYNOMIAL(1, 6, 8) / POLYNOMIAL(2, 4, -10, 7) << endl;
cout << POLYNOMIAL(1, 6, 8) % POLYNOMIAL(2, 4, -10, 7) << endl;
cout << POLYNOMIAL(1, 9, 5) / POLYNOMIAL(2, 3, 2, -10) << endl;
cout << POLYNOMIAL(1, 9, 5) % POLYNOMIAL(2, 3, 2, -10) << endl;
cout << POLYNOMIAL(3, -5, -5, 0, 1) / POLYNOMIAL(3, 8, -5, 0, -8) << endl;
cout << POLYNOMIAL(3, -5, -5, 0, 1) % POLYNOMIAL(3, 8, -5, 0, -8) << endl;
cout << POLYNOMIAL(2, 0, 3, -5) / POLYNOMIAL(3, 5, -9, 2, -9) << endl;
cout << POLYNOMIAL(2, 0, 3, -5) % POLYNOMIAL(3, 5, -9, 2, -9) << endl;
cout << POLYNOMIAL(2, -2, 0, -10) / POLYNOMIAL(2, 9, 6, -1) << endl;
cout << POLYNOMIAL(2, -2, 0, -10) % POLYNOMIAL(2, 9, 6, -1) << endl;
cout << POLYNOMIAL(3, 7, 1, -9, 1) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(3, 7, 1, -9, 1) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(2, -10, -10, 8) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, -10, -10, 8) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(3, -10, -4, -5, -9) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(3, -10, -4, -5, -9) << endl;
cout << POLYNOMIAL(3, -3, 4, -7, -9) / POLYNOMIAL(2, 4, 4, 9) << endl;
cout << POLYNOMIAL(3, -3, 4, -7, -9) % POLYNOMIAL(2, 4, 4, 9) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(3, 7, 8, -10, 4) / POLYNOMIAL(3, 8, -7, 10, -1) << endl;
cout << POLYNOMIAL(3, 7, 8, -10, 4) % POLYNOMIAL(3, 8, -7, 10, -1) << endl;
cout << POLYNOMIAL(3, 10, 3, -1, 9) / POLYNOMIAL(1, 8, -2) << endl;
cout << POLYNOMIAL(3, 10, 3, -1, 9) % POLYNOMIAL(1, 8, -2) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(3, 4, 9, -9, 4) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(3, 4, 9, -9, 4) << endl;
cout << POLYNOMIAL(1, -9, -3) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(1, -9, -3) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(1, 3, -3) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(1, 3, -3) << endl;
cout << POLYNOMIAL(3, -3, -6, -10, -1) / POLYNOMIAL(2, -8, 6, -8) << endl;
cout << POLYNOMIAL(3, -3, -6, -10, -1) % POLYNOMIAL(2, -8, 6, -8) << endl;
cout << POLYNOMIAL(1, 3, 3) / POLYNOMIAL(3, -7, 4, -9, -10) << endl;
cout << POLYNOMIAL(1, 3, 3) % POLYNOMIAL(3, -7, 4, -9, -10) << endl;
cout << POLYNOMIAL(3, 6, 9, 5, 4) / POLYNOMIAL(1, 7, 10) << endl;
cout << POLYNOMIAL(3, 6, 9, 5, 4) % POLYNOMIAL(1, 7, 10) << endl;
cout << POLYNOMIAL(1, 6, 4) / POLYNOMIAL(1, 5, -1) << endl;
cout << POLYNOMIAL(1, 6, 4) % POLYNOMIAL(1, 5, -1) << endl;
cout << POLYNOMIAL(3, -9, 1, -8, 6) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -9, 1, -8, 6) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -9, 9, 8, 10) / POLYNOMIAL(3, 1, 5, -3, 5) << endl;
cout << POLYNOMIAL(3, -9, 9, 8, 10) % POLYNOMIAL(3, 1, 5, -3, 5) << endl;
cout << POLYNOMIAL(1, 4, 10) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 4, 10) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, -4, -9) / POLYNOMIAL(1, 3, -10) << endl;
cout << POLYNOMIAL(1, -4, -9) % POLYNOMIAL(1, 3, -10) << endl;
cout << POLYNOMIAL(2, 7, 0, 2) / POLYNOMIAL(2, 1, 1, -6) << endl;
cout << POLYNOMIAL(2, 7, 0, 2) % POLYNOMIAL(2, 1, 1, -6) << endl;
cout << POLYNOMIAL(3, 8, -2, -4, 5) / POLYNOMIAL(2, 9, 7, 0) << endl;
cout << POLYNOMIAL(3, 8, -2, -4, 5) % POLYNOMIAL(2, 9, 7, 0) << endl;
cout << POLYNOMIAL(2, 9, 0, 5) / POLYNOMIAL(1, -8, 8) << endl;
cout << POLYNOMIAL(2, 9, 0, 5) % POLYNOMIAL(1, -8, 8) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(1, 6, 1) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(1, 6, 1) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -6, 6, 3, -8) / POLYNOMIAL(1, 5, -10) << endl;
cout << POLYNOMIAL(3, -6, 6, 3, -8) % POLYNOMIAL(1, 5, -10) << endl;
cout << POLYNOMIAL(2, 3, 0, 10) / POLYNOMIAL(2, -5, -5, 3) << endl;
cout << POLYNOMIAL(2, 3, 0, 10) % POLYNOMIAL(2, -5, -5, 3) << endl;
cout << POLYNOMIAL(0, 2) / POLYNOMIAL(3, -10, 8, -6, 7) << endl;
cout << POLYNOMIAL(0, 2) % POLYNOMIAL(3, -10, 8, -6, 7) << endl;
cout << POLYNOMIAL(2, -3, -8, 7) / POLYNOMIAL(1, -2, -5) << endl;
cout << POLYNOMIAL(2, -3, -8, 7) % POLYNOMIAL(1, -2, -5) << endl;
cout << POLYNOMIAL(0, 6) / POLYNOMIAL(2, -10, 10, 8) << endl;
cout << POLYNOMIAL(0, 6) % POLYNOMIAL(2, -10, 10, 8) << endl;
cout << POLYNOMIAL(2, -5, -10, -8) / POLYNOMIAL(3, 7, 5, 4, -2) << endl;
cout << POLYNOMIAL(2, -5, -10, -8) % POLYNOMIAL(3, 7, 5, 4, -2) << endl;
cout << POLYNOMIAL(1, -7, 6) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(1, -7, 6) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -9, -2, 8, 2) / POLYNOMIAL(3, 8, 7, -3, 6) << endl;
cout << POLYNOMIAL(3, -9, -2, 8, 2) % POLYNOMIAL(3, 8, 7, -3, 6) << endl;
cout << POLYNOMIAL(1, -2, 6) / POLYNOMIAL(3, 9, -3, 0, -2) << endl;
cout << POLYNOMIAL(1, -2, 6) % POLYNOMIAL(3, 9, -3, 0, -2) << endl;
cout << POLYNOMIAL(1, 7, -3) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(1, 7, -3) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(2, -5, -3, -10) / POLYNOMIAL(1, 10, 4) << endl;
cout << POLYNOMIAL(2, -5, -3, -10) % POLYNOMIAL(1, 10, 4) << endl;
cout << POLYNOMIAL(1, -5, -10) / POLYNOMIAL(2, -4, -1, 3) << endl;
cout << POLYNOMIAL(1, -5, -10) % POLYNOMIAL(2, -4, -1, 3) << endl;
cout << POLYNOMIAL(1, 0, -8) / POLYNOMIAL(2, 8, -8, -7) << endl;
cout << POLYNOMIAL(1, 0, -8) % POLYNOMIAL(2, 8, -8, -7) << endl;
cout << POLYNOMIAL(2, -4, 6, 4) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, -4, 6, 4) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(3, 0, -3, -8, -5) / POLYNOMIAL(1, 5, 1) << endl;
cout << POLYNOMIAL(3, 0, -3, -8, -5) % POLYNOMIAL(1, 5, 1) << endl;
cout << POLYNOMIAL(3, 9, -2, 5, -10) / POLYNOMIAL(1, -6, 1) << endl;
cout << POLYNOMIAL(3, 9, -2, 5, -10) % POLYNOMIAL(1, -6, 1) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(1, 0, 2) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(1, 0, 2) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(1, -8, 9) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(1, -8, 9) << endl;
cout << POLYNOMIAL(2, 9, 4, 0) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(2, 9, 4, 0) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(2, -6, 2, 8) / POLYNOMIAL(1, 3, 4) << endl;
cout << POLYNOMIAL(2, -6, 2, 8) % POLYNOMIAL(1, 3, 4) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(1, -1, 4) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(1, -1, 4) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(2, -8, -4, -8) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(2, -8, -4, -8) << endl;
cout << POLYNOMIAL(1, -10, 9) / POLYNOMIAL(3, 0, -2, -9, -2) << endl;
cout << POLYNOMIAL(1, -10, 9) % POLYNOMIAL(3, 0, -2, -9, -2) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(2, -4, 6, -3) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(2, -4, 6, -3) << endl;
cout << POLYNOMIAL(1, -7, -2) / POLYNOMIAL(2, 4, -2, -8) << endl;
cout << POLYNOMIAL(1, -7, -2) % POLYNOMIAL(2, 4, -2, -8) << endl;
cout << POLYNOMIAL(3, 8, 1, -10, 1) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(3, 8, 1, -10, 1) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(2, 5, -6, -7) / POLYNOMIAL(2, -8, 6, 2) << endl;
cout << POLYNOMIAL(2, 5, -6, -7) % POLYNOMIAL(2, -8, 6, 2) << endl;
cout << POLYNOMIAL(3, 1, -4, 0, 10) / POLYNOMIAL(1, 9, 10) << endl;
cout << POLYNOMIAL(3, 1, -4, 0, 10) % POLYNOMIAL(1, 9, 10) << endl;
cout << POLYNOMIAL(3, -2, -4, 7, -7) / POLYNOMIAL(3, -8, -10, 6, -5) << endl;
cout << POLYNOMIAL(3, -2, -4, 7, -7) % POLYNOMIAL(3, -8, -10, 6, -5) << endl;
cout << POLYNOMIAL(3, 9, -1, 1, 3) / POLYNOMIAL(1, 8, -2) << endl;
cout << POLYNOMIAL(3, 9, -1, 1, 3) % POLYNOMIAL(1, 8, -2) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(2, -5, 6, 3) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(2, -5, 6, 3) << endl;
cout << POLYNOMIAL(3, -4, -5, 4, -3) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(3, -4, -5, 4, -3) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(2, 5, 5, 9) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(2, 5, 5, 9) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(3, 4, 10, -4, -3) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(3, 4, 10, -4, -3) << endl;
cout << POLYNOMIAL(1, 6, 2) / POLYNOMIAL(3, 10, -2, -2, 6) << endl;
cout << POLYNOMIAL(1, 6, 2) % POLYNOMIAL(3, 10, -2, -2, 6) << endl;
cout << POLYNOMIAL(2, -5, -10, 10) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, -5, -10, 10) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, 4, 6, 4) / POLYNOMIAL(3, -1, -1, -8, -2) << endl;
cout << POLYNOMIAL(2, 4, 6, 4) % POLYNOMIAL(3, -1, -1, -8, -2) << endl;
cout << POLYNOMIAL(1, -4, 8) / POLYNOMIAL(1, -7, 2) << endl;
cout << POLYNOMIAL(1, -4, 8) % POLYNOMIAL(1, -7, 2) << endl;
cout << POLYNOMIAL(3, -10, -4, 7, 4) / POLYNOMIAL(3, -1, 5, -10, -9) << endl;
cout << POLYNOMIAL(3, -10, -4, 7, 4) % POLYNOMIAL(3, -1, 5, -10, -9) << endl;
cout << POLYNOMIAL(2, 10, -9, -3) / POLYNOMIAL(2, 9, 10, -7) << endl;
cout << POLYNOMIAL(2, 10, -9, -3) % POLYNOMIAL(2, 9, 10, -7) << endl;
cout << POLYNOMIAL(2, 4, -3, -2) / POLYNOMIAL(3, 1, -7, -6, -4) << endl;
cout << POLYNOMIAL(2, 4, -3, -2) % POLYNOMIAL(3, 1, -7, -6, -4) << endl;
cout << POLYNOMIAL(2, -7, 5, 7) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(2, -7, 5, 7) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(2, -9, -2, 7) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(2, -9, -2, 7) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, 7, -1, -8, 4) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, 7, -1, -8, 4) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, 9, 4, 8, -5) / POLYNOMIAL(1, -6, 10) << endl;
cout << POLYNOMIAL(3, 9, 4, 8, -5) % POLYNOMIAL(1, -6, 10) << endl;
cout << POLYNOMIAL(1, -8, 0) / POLYNOMIAL(2, 5, -9, 2) << endl;
cout << POLYNOMIAL(1, -8, 0) % POLYNOMIAL(2, 5, -9, 2) << endl;
cout << POLYNOMIAL(3, 7, -2, -10, 4) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(3, 7, -2, -10, 4) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(2, 10, -1, 9) / POLYNOMIAL(2, 0, -4, 6) << endl;
cout << POLYNOMIAL(2, 10, -1, 9) % POLYNOMIAL(2, 0, -4, 6) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(3, -6, -8, 4, -9) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(3, -6, -8, 4, -9) << endl;
cout << POLYNOMIAL(1, 8, -3) / POLYNOMIAL(3, 2, 4, -6, 6) << endl;
cout << POLYNOMIAL(1, 8, -3) % POLYNOMIAL(3, 2, 4, -6, 6) << endl;
cout << POLYNOMIAL(1, 4, 5) / POLYNOMIAL(2, 7, -8, -9) << endl;
cout << POLYNOMIAL(1, 4, 5) % POLYNOMIAL(2, 7, -8, -9) << endl;
cout << POLYNOMIAL(2, -2, -1, -7) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, -2, -1, -7) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, 1, 9, -3) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, 1, 9, -3) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, -9, 2, -8) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(2, -9, 2, -8) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(1, 4, -5) / POLYNOMIAL(3, 8, 0, -7, -5) << endl;
cout << POLYNOMIAL(1, 4, -5) % POLYNOMIAL(3, 8, 0, -7, -5) << endl;
cout << POLYNOMIAL(2, -6, 4, 10) / POLYNOMIAL(3, -6, 7, 9, -8) << endl;
cout << POLYNOMIAL(2, -6, 4, 10) % POLYNOMIAL(3, -6, 7, 9, -8) << endl;
cout << POLYNOMIAL(3, -1, 2, -10, -9) / POLYNOMIAL(2, 1, -8, 2) << endl;
cout << POLYNOMIAL(3, -1, 2, -10, -9) % POLYNOMIAL(2, 1, -8, 2) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(1, 6, -10) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(1, 6, -10) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(2, 0, 10, -7) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(2, 0, 10, -7) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(3, 5, 10, -3, -4) / POLYNOMIAL(3, 6, 8, -3, -6) << endl;
cout << POLYNOMIAL(3, 5, 10, -3, -4) % POLYNOMIAL(3, 6, 8, -3, -6) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(2, -3, 6, 6) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(2, -3, 6, 6) << endl;
cout << POLYNOMIAL(3, 10, 4, -5, 7) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(3, 10, 4, -5, 7) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(3, 4, -8, 3, -10) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(3, 4, -8, 3, -10) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, -3, 10, 8) / POLYNOMIAL(2, -3, -5, -3) << endl;
cout << POLYNOMIAL(2, -3, 10, 8) % POLYNOMIAL(2, -3, -5, -3) << endl;
cout << POLYNOMIAL(1, 8, 9) / POLYNOMIAL(3, -1, -5, -8, 9) << endl;
cout << POLYNOMIAL(1, 8, 9) % POLYNOMIAL(3, -1, -5, -8, 9) << endl;
cout << POLYNOMIAL(1, 3, -5) / POLYNOMIAL(1, 10, -1) << endl;
cout << POLYNOMIAL(1, 3, -5) % POLYNOMIAL(1, 10, -1) << endl;
cout << POLYNOMIAL(1, 8, 7) / POLYNOMIAL(2, -5, -4, -4) << endl;
cout << POLYNOMIAL(1, 8, 7) % POLYNOMIAL(2, -5, -4, -4) << endl;
cout << POLYNOMIAL(1, 9, 10) / POLYNOMIAL(2, -4, -6, 4) << endl;
cout << POLYNOMIAL(1, 9, 10) % POLYNOMIAL(2, -4, -6, 4) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(1, 7, 3) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(1, 7, 3) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(3, 1, 9, -4, -7) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(3, 1, 9, -4, -7) << endl;
cout << POLYNOMIAL(3, 0, -3, 2, 1) / POLYNOMIAL(2, -5, -7, -6) << endl;
cout << POLYNOMIAL(3, 0, -3, 2, 1) % POLYNOMIAL(2, -5, -7, -6) << endl;
cout << POLYNOMIAL(0, 5) / POLYNOMIAL(3, 7, -3, 2, -1) << endl;
cout << POLYNOMIAL(0, 5) % POLYNOMIAL(3, 7, -3, 2, -1) << endl;
cout << POLYNOMIAL(1, 0, 5) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(1, 0, 5) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(1, -6, 4) / POLYNOMIAL(3, -6, -1, 6, 4) << endl;
cout << POLYNOMIAL(1, -6, 4) % POLYNOMIAL(3, -6, -1, 6, 4) << endl;
cout << POLYNOMIAL(2, 3, -4, 3) / POLYNOMIAL(2, -2, 9, -1) << endl;
cout << POLYNOMIAL(2, 3, -4, 3) % POLYNOMIAL(2, -2, 9, -1) << endl;
cout << POLYNOMIAL(1, -9, 3) / POLYNOMIAL(1, 4, -8) << endl;
cout << POLYNOMIAL(1, -9, 3) % POLYNOMIAL(1, 4, -8) << endl;
cout << POLYNOMIAL(2, 5, -10, 10) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(2, 5, -10, 10) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(3, -2, -9, 10, -1) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -2, -9, 10, -1) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -10, 1, -9, 9) / POLYNOMIAL(2, 8, -9, 8) << endl;
cout << POLYNOMIAL(3, -10, 1, -9, 9) % POLYNOMIAL(2, 8, -9, 8) << endl;
cout << POLYNOMIAL(1, 7, -10) / POLYNOMIAL(2, -10, 8, -4) << endl;
cout << POLYNOMIAL(1, 7, -10) % POLYNOMIAL(2, -10, 8, -4) << endl;
cout << POLYNOMIAL(1, 1, 10) / POLYNOMIAL(3, 10, 2, 6, -3) << endl;
cout << POLYNOMIAL(1, 1, 10) % POLYNOMIAL(3, 10, 2, 6, -3) << endl;
cout << POLYNOMIAL(2, -7, 7, -6) / POLYNOMIAL(1, 8, -1) << endl;
cout << POLYNOMIAL(2, -7, 7, -6) % POLYNOMIAL(1, 8, -1) << endl;
cout << POLYNOMIAL(3, 1, -10, -5, 9) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, 1, -10, -5, 9) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(2, 2, -6, -10) / POLYNOMIAL(3, 2, -8, -7, 5) << endl;
cout << POLYNOMIAL(2, 2, -6, -10) % POLYNOMIAL(3, 2, -8, -7, 5) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(1, -7, -2) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(1, -7, -2) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(1, 2, 5) / POLYNOMIAL(3, -2, 8, 1, -3) << endl;
cout << POLYNOMIAL(1, 2, 5) % POLYNOMIAL(3, -2, 8, 1, -3) << endl;
cout << POLYNOMIAL(2, 0, -3, -1) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(2, 0, -3, -1) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(0, 6) / POLYNOMIAL(2, -10, 4, -9) << endl;
cout << POLYNOMIAL(0, 6) % POLYNOMIAL(2, -10, 4, -9) << endl;
cout << POLYNOMIAL(1, 7, -8) / POLYNOMIAL(3, 4, -9, -1, -5) << endl;
cout << POLYNOMIAL(1, 7, -8) % POLYNOMIAL(3, 4, -9, -1, -5) << endl;
cout << POLYNOMIAL(1, 5, 1) / POLYNOMIAL(1, 2, 10) << endl;
cout << POLYNOMIAL(1, 5, 1) % POLYNOMIAL(1, 2, 10) << endl;
cout << POLYNOMIAL(2, 6, 2, 0) / POLYNOMIAL(2, -1, 10, 8) << endl;
cout << POLYNOMIAL(2, 6, 2, 0) % POLYNOMIAL(2, -1, 10, 8) << endl;
cout << POLYNOMIAL(3, -1, 3, 6, 2) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(3, -1, 3, 6, 2) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(3, 9, -4, 5, -10) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(3, 9, -4, 5, -10) << endl;
cout << POLYNOMIAL(2, 1, -3, -9) / POLYNOMIAL(1, 5, -7) << endl;
cout << POLYNOMIAL(2, 1, -3, -9) % POLYNOMIAL(1, 5, -7) << endl;
cout << POLYNOMIAL(2, 2, 4, -9) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(2, 2, 4, -9) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 1, 6) / POLYNOMIAL(3, 6, 9, 5, -4) << endl;
cout << POLYNOMIAL(1, 1, 6) % POLYNOMIAL(3, 6, 9, 5, -4) << endl;
cout << POLYNOMIAL(3, 1, -5, 2, -6) / POLYNOMIAL(1, 6, -5) << endl;
cout << POLYNOMIAL(3, 1, -5, 2, -6) % POLYNOMIAL(1, 6, -5) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(2, 6, 1, -3) / POLYNOMIAL(2, 2, -1, 5) << endl;
cout << POLYNOMIAL(2, 6, 1, -3) % POLYNOMIAL(2, 2, -1, 5) << endl;
cout << POLYNOMIAL(3, -10, 2, 5, 10) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(3, -10, 2, 5, 10) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(3, -5, 4, 3, 6) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(3, -5, 4, 3, 6) << endl;
cout << POLYNOMIAL(3, 10, 1, 10, -1) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(3, 10, 1, 10, -1) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(2, 2, -5, -6) / POLYNOMIAL(2, 9, 5, 6) << endl;
cout << POLYNOMIAL(2, 2, -5, -6) % POLYNOMIAL(2, 9, 5, 6) << endl;
cout << POLYNOMIAL(1, 1, 9) / POLYNOMIAL(3, 6, -7, -1, -2) << endl;
cout << POLYNOMIAL(1, 1, 9) % POLYNOMIAL(3, 6, -7, -1, -2) << endl;
cout << POLYNOMIAL(1, -2, 9) / POLYNOMIAL(1, 6, 10) << endl;
cout << POLYNOMIAL(1, -2, 9) % POLYNOMIAL(1, 6, 10) << endl;
cout << POLYNOMIAL(1, 5, 7) / POLYNOMIAL(2, -6, -6, -9) << endl;
cout << POLYNOMIAL(1, 5, 7) % POLYNOMIAL(2, -6, -6, -9) << endl;
cout << POLYNOMIAL(3, 5, 10, 5, 4) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(3, 5, 10, 5, 4) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(2, 8, 9, 3) / POLYNOMIAL(3, -10, -2, 10, 3) << endl;
cout << POLYNOMIAL(2, 8, 9, 3) % POLYNOMIAL(3, -10, -2, 10, 3) << endl;
cout << POLYNOMIAL(1, -5, -8) / POLYNOMIAL(3, 10, 4, -3, -2) << endl;
cout << POLYNOMIAL(1, -5, -8) % POLYNOMIAL(3, 10, 4, -3, -2) << endl;
cout << POLYNOMIAL(3, 2, -9, 1, -3) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, 2, -9, 1, -3) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -9, -6, 4, 10) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(3, -9, -6, 4, 10) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(3, 4, -2, -6, 4) / POLYNOMIAL(1, -2, -4) << endl;
cout << POLYNOMIAL(3, 4, -2, -6, 4) % POLYNOMIAL(1, -2, -4) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(1, -1, -4) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(1, -1, -4) << endl;
cout << POLYNOMIAL(1, 10, -7) / POLYNOMIAL(1, -10, 1) << endl;
cout << POLYNOMIAL(1, 10, -7) % POLYNOMIAL(1, -10, 1) << endl;
cout << POLYNOMIAL(3, 9, -1, 5, -9) / POLYNOMIAL(1, 9, 8) << endl;
cout << POLYNOMIAL(3, 9, -1, 5, -9) % POLYNOMIAL(1, 9, 8) << endl;
cout << POLYNOMIAL(2, -10, -9, 9) / POLYNOMIAL(3, 5, 7, -10, -8) << endl;
cout << POLYNOMIAL(2, -10, -9, 9) % POLYNOMIAL(3, 5, 7, -10, -8) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(3, -9, -2, -2, 6) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(3, -9, -2, -2, 6) << endl;
cout << POLYNOMIAL(1, 1, 5) / POLYNOMIAL(2, 6, 4, 7) << endl;
cout << POLYNOMIAL(1, 1, 5) % POLYNOMIAL(2, 6, 4, 7) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(3, 0, -2, 4, 0) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(3, 0, -2, 4, 0) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(1, 3, 6) / POLYNOMIAL(3, 0, 4, 10, -1) << endl;
cout << POLYNOMIAL(1, 3, 6) % POLYNOMIAL(3, 0, 4, 10, -1) << endl;
cout << POLYNOMIAL(1, 7, 7) / POLYNOMIAL(1, 4, -5) << endl;
cout << POLYNOMIAL(1, 7, 7) % POLYNOMIAL(1, 4, -5) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(2, 3, -7, -7) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(2, 3, -7, -7) << endl;
cout << POLYNOMIAL(3, -1, -9, 6, 7) / POLYNOMIAL(2, 2, 0, 9) << endl;
cout << POLYNOMIAL(3, -1, -9, 6, 7) % POLYNOMIAL(2, 2, 0, 9) << endl;
cout << POLYNOMIAL(1, -3, 5) / POLYNOMIAL(2, -7, 1, 5) << endl;
cout << POLYNOMIAL(1, -3, 5) % POLYNOMIAL(2, -7, 1, 5) << endl;
cout << POLYNOMIAL(2, 3, 4, -6) / POLYNOMIAL(2, 4, -7, -5) << endl;
cout << POLYNOMIAL(2, 3, 4, -6) % POLYNOMIAL(2, 4, -7, -5) << endl;
cout << POLYNOMIAL(2, -3, 5, -5) / POLYNOMIAL(2, -10, 3, 10) << endl;
cout << POLYNOMIAL(2, -3, 5, -5) % POLYNOMIAL(2, -10, 3, 10) << endl;
cout << POLYNOMIAL(3, -5, -7, 1, -5) / POLYNOMIAL(1, 3, 7) << endl;
cout << POLYNOMIAL(3, -5, -7, 1, -5) % POLYNOMIAL(1, 3, 7) << endl;
cout << POLYNOMIAL(3, -1, 8, -6, -6) / POLYNOMIAL(2, 9, 7, -4) << endl;
cout << POLYNOMIAL(3, -1, 8, -6, -6) % POLYNOMIAL(2, 9, 7, -4) << endl;
cout << POLYNOMIAL(1, 0, 3) / POLYNOMIAL(2, 9, -7, 0) << endl;
cout << POLYNOMIAL(1, 0, 3) % POLYNOMIAL(2, 9, -7, 0) << endl;
cout << POLYNOMIAL(3, 4, 3, 4, 1) / POLYNOMIAL(2, 6, -5, 0) << endl;
cout << POLYNOMIAL(3, 4, 3, 4, 1) % POLYNOMIAL(2, 6, -5, 0) << endl;
cout << POLYNOMIAL(2, -8, 6, 1) / POLYNOMIAL(1, -6, 6) << endl;
cout << POLYNOMIAL(2, -8, 6, 1) % POLYNOMIAL(1, -6, 6) << endl;
cout << POLYNOMIAL(3, 6, 3, 5, 7) / POLYNOMIAL(3, -4, 5, 9, 1) << endl;
cout << POLYNOMIAL(3, 6, 3, 5, 7) % POLYNOMIAL(3, -4, 5, 9, 1) << endl;
cout << POLYNOMIAL(3, -3, -6, -3, -5) / POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(3, -3, -6, -3, -5) % POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(2, 8, 9, 8) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(2, 8, 9, 8) << endl;
cout << POLYNOMIAL(2, 3, -10, 8) / POLYNOMIAL(1, -9, 3) << endl;
cout << POLYNOMIAL(2, 3, -10, 8) % POLYNOMIAL(1, -9, 3) << endl;
cout << POLYNOMIAL(3, 3, 6, 4, -9) / POLYNOMIAL(1, -1, -8) << endl;
cout << POLYNOMIAL(3, 3, 6, 4, -9) % POLYNOMIAL(1, -1, -8) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(1, 4, -4) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(1, 4, -4) << endl;
cout << POLYNOMIAL(3, 7, 1, 1, 10) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, 7, 1, 1, 10) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, 3, -7, -10, 5) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(3, 3, -7, -10, 5) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(2, 4, 6, 6) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(2, 4, 6, 6) << endl;
cout << POLYNOMIAL(2, 7, 9, 10) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(2, 7, 9, 10) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(3, 7, -1, -6, 1) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(3, 7, -1, -6, 1) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(3, 1, 1, -1, 5) / POLYNOMIAL(3, -7, 3, -2, 2) << endl;
cout << POLYNOMIAL(3, 1, 1, -1, 5) % POLYNOMIAL(3, -7, 3, -2, 2) << endl;
cout << POLYNOMIAL(3, -6, -2, 4, 2) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(3, -6, -2, 4, 2) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(3, 1, -1, 8, -4) / POLYNOMIAL(3, 3, 5, 8, 3) << endl;
cout << POLYNOMIAL(3, 1, -1, 8, -4) % POLYNOMIAL(3, 3, 5, 8, 3) << endl;
cout << POLYNOMIAL(1, 2, 10) / POLYNOMIAL(2, 8, 7, 4) << endl;
cout << POLYNOMIAL(1, 2, 10) % POLYNOMIAL(2, 8, 7, 4) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(1, -10, 8) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(1, -10, 8) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(3, -7, -6, 8, 6) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(3, -7, -6, 8, 6) << endl;
cout << POLYNOMIAL(2, 5, 0, 3) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(2, 5, 0, 3) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(3, 6, 8, -2, -4) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(3, 6, 8, -2, -4) << endl;
cout << POLYNOMIAL(1, 9, -3) / POLYNOMIAL(1, 8, 10) << endl;
cout << POLYNOMIAL(1, 9, -3) % POLYNOMIAL(1, 8, 10) << endl;
cout << POLYNOMIAL(2, -4, -1, 5) / POLYNOMIAL(2, 7, 5, -5) << endl;
cout << POLYNOMIAL(2, -4, -1, 5) % POLYNOMIAL(2, 7, 5, -5) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(2, -4, 8, 5) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(2, -4, 8, 5) << endl;
cout << POLYNOMIAL(1, -6, -1) / POLYNOMIAL(2, -4, 0, -6) << endl;
cout << POLYNOMIAL(1, -6, -1) % POLYNOMIAL(2, -4, 0, -6) << endl;
cout << POLYNOMIAL(2, 10, -10, -1) / POLYNOMIAL(2, 0, 6, 8) << endl;
cout << POLYNOMIAL(2, 10, -10, -1) % POLYNOMIAL(2, 0, 6, 8) << endl;
cout << POLYNOMIAL(2, 0, -8, 10) / POLYNOMIAL(2, -10, -7, 5) << endl;
cout << POLYNOMIAL(2, 0, -8, 10) % POLYNOMIAL(2, -10, -7, 5) << endl;
cout << POLYNOMIAL(2, -5, 0, -9) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(2, -5, 0, -9) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(2, 4, -6, 3) / POLYNOMIAL(1, 7, -10) << endl;
cout << POLYNOMIAL(2, 4, -6, 3) % POLYNOMIAL(1, 7, -10) << endl;
cout << POLYNOMIAL(2, -4, 4, 0) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(2, -4, 4, 0) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(3, -1, 5, -6, 1) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(3, -1, 5, -6, 1) << endl;
cout << POLYNOMIAL(2, 6, 3, -2) / POLYNOMIAL(1, 10, -5) << endl;
cout << POLYNOMIAL(2, 6, 3, -2) % POLYNOMIAL(1, 10, -5) << endl;
cout << POLYNOMIAL(2, 6, -4, 4) / POLYNOMIAL(1, 9, 8) << endl;
cout << POLYNOMIAL(2, 6, -4, 4) % POLYNOMIAL(1, 9, 8) << endl;
cout << POLYNOMIAL(2, 4, 4, -9) / POLYNOMIAL(2, 6, 3, 0) << endl;
cout << POLYNOMIAL(2, 4, 4, -9) % POLYNOMIAL(2, 6, 3, 0) << endl;
cout << POLYNOMIAL(3, -2, -1, 9, 3) / POLYNOMIAL(1, -6, 2) << endl;
cout << POLYNOMIAL(3, -2, -1, 9, 3) % POLYNOMIAL(1, -6, 2) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(2, -1, -7, -10) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(2, -1, -7, -10) << endl;
cout << POLYNOMIAL(3, 1, -7, 0, 3) / POLYNOMIAL(2, -4, 9, -1) << endl;
cout << POLYNOMIAL(3, 1, -7, 0, 3) % POLYNOMIAL(2, -4, 9, -1) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(2, -3, -5, -6) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(2, -3, -5, -6) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(1, 4, -9) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(1, 4, -9) << endl;
cout << POLYNOMIAL(1, 9, 6) / POLYNOMIAL(2, 9, 10, 7) << endl;
cout << POLYNOMIAL(1, 9, 6) % POLYNOMIAL(2, 9, 10, 7) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(3, -10, -10, -6, 1) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(3, -10, -10, -6, 1) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(2, 1, -9, 0) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(2, 1, -9, 0) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(1, -8, 3) / POLYNOMIAL(3, 0, -8, 3, -8) << endl;
cout << POLYNOMIAL(1, -8, 3) % POLYNOMIAL(3, 0, -8, 3, -8) << endl;
cout << POLYNOMIAL(1, 0, -4) / POLYNOMIAL(2, 7, 9, 9) << endl;
cout << POLYNOMIAL(1, 0, -4) % POLYNOMIAL(2, 7, 9, 9) << endl;
cout << POLYNOMIAL(1, 9, 2) / POLYNOMIAL(2, -10, 7, 6) << endl;
cout << POLYNOMIAL(1, 9, 2) % POLYNOMIAL(2, -10, 7, 6) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(3, 0, 4, -7, 0) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(3, 0, 4, -7, 0) << endl;
cout << POLYNOMIAL(2, -4, -4, 9) / POLYNOMIAL(2, 8, -5, 10) << endl;
cout << POLYNOMIAL(2, -4, -4, 9) % POLYNOMIAL(2, 8, -5, 10) << endl;
cout << POLYNOMIAL(1, -1, -5) / POLYNOMIAL(1, 4, -10) << endl;
cout << POLYNOMIAL(1, -1, -5) % POLYNOMIAL(1, 4, -10) << endl;
cout << POLYNOMIAL(3, -9, -7, -7, 1) / POLYNOMIAL(2, 4, -2, 6) << endl;
cout << POLYNOMIAL(3, -9, -7, -7, 1) % POLYNOMIAL(2, 4, -2, 6) << endl;
cout << POLYNOMIAL(3, 7, -4, 2, -8) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, 7, -4, 2, -8) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -9, 3, 7, -7) / POLYNOMIAL(2, -6, 4, -3) << endl;
cout << POLYNOMIAL(3, -9, 3, 7, -7) % POLYNOMIAL(2, -6, 4, -3) << endl;
cout << POLYNOMIAL(3, 9, -10, 6, 1) / POLYNOMIAL(3, 10, -2, -6, -1) << endl;
cout << POLYNOMIAL(3, 9, -10, 6, 1) % POLYNOMIAL(3, 10, -2, -6, -1) << endl;
cout << POLYNOMIAL(1, 0, 2) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(1, 0, 2) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(1, 1, 1) / POLYNOMIAL(1, -10, 3) << endl;
cout << POLYNOMIAL(1, 1, 1) % POLYNOMIAL(1, -10, 3) << endl;
cout << POLYNOMIAL(2, 1, 9, 7) / POLYNOMIAL(3, -5, -10, -6, 5) << endl;
cout << POLYNOMIAL(2, 1, 9, 7) % POLYNOMIAL(3, -5, -10, -6, 5) << endl;
cout << POLYNOMIAL(2, -4, 1, 6) / POLYNOMIAL(2, -3, 4, -10) << endl;
cout << POLYNOMIAL(2, -4, 1, 6) % POLYNOMIAL(2, -3, 4, -10) << endl;
cout << POLYNOMIAL(1, -10, 1) / POLYNOMIAL(2, -4, 1, 9) << endl;
cout << POLYNOMIAL(1, -10, 1) % POLYNOMIAL(2, -4, 1, 9) << endl;
cout << POLYNOMIAL(2, -2, 7, 2) / POLYNOMIAL(3, 0, 6, -9, 4) << endl;
cout << POLYNOMIAL(2, -2, 7, 2) % POLYNOMIAL(3, 0, 6, -9, 4) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(1, 0, -5) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(1, 0, -5) << endl;
cout << POLYNOMIAL(1, 2, -2) / POLYNOMIAL(1, 0, 6) << endl;
cout << POLYNOMIAL(1, 2, -2) % POLYNOMIAL(1, 0, 6) << endl;
cout << POLYNOMIAL(1, -9, -8) / POLYNOMIAL(1, 8, -1) << endl;
cout << POLYNOMIAL(1, -9, -8) % POLYNOMIAL(1, 8, -1) << endl;
cout << POLYNOMIAL(3, -10, 1, 0, 10) / POLYNOMIAL(2, -6, -10, 2) << endl;
cout << POLYNOMIAL(3, -10, 1, 0, 10) % POLYNOMIAL(2, -6, -10, 2) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(2, -8, -6, -2) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(2, -8, -6, -2) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(2, -10, -7, -9) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(2, -10, -7, -9) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(2, 6, 0, -5) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(2, 6, 0, -5) << endl;
cout << POLYNOMIAL(1, -9, 4) / POLYNOMIAL(3, 8, -9, 4, 8) << endl;
cout << POLYNOMIAL(1, -9, 4) % POLYNOMIAL(3, 8, -9, 4, 8) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(1, -1, -9) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(1, -1, -9) << endl;
cout << POLYNOMIAL(1, -8, 8) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, -8, 8) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(2, -2, -3, -7) / POLYNOMIAL(1, -8, -5) << endl;
cout << POLYNOMIAL(2, -2, -3, -7) % POLYNOMIAL(1, -8, -5) << endl;
cout << POLYNOMIAL(2, -5, 8, -5) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(2, -5, 8, -5) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(1, 7, 9) / POLYNOMIAL(2, -9, -3, 5) << endl;
cout << POLYNOMIAL(1, 7, 9) % POLYNOMIAL(2, -9, -3, 5) << endl;
cout << POLYNOMIAL(1, 0, -1) / POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(1, 0, -1) % POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(3, 9, -10, 3, 10) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, 9, -10, 3, 10) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(3, -5, -5, -2, -6) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(3, -5, -5, -2, -6) << endl;
cout << POLYNOMIAL(1, -3, -4) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, -3, -4) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, -2, -8) / POLYNOMIAL(3, 5, -2, 6, 6) << endl;
cout << POLYNOMIAL(1, -2, -8) % POLYNOMIAL(3, 5, -2, 6, 6) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(1, -8, 7) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(1, -8, 7) << endl;
cout << POLYNOMIAL(1, -6, -7) / POLYNOMIAL(2, -5, -6, -2) << endl;
cout << POLYNOMIAL(1, -6, -7) % POLYNOMIAL(2, -5, -6, -2) << endl;
cout << POLYNOMIAL(3, -2, -5, -5, 7) / POLYNOMIAL(3, -2, -2, 0, 6) << endl;
cout << POLYNOMIAL(3, -2, -5, -5, 7) % POLYNOMIAL(3, -2, -2, 0, 6) << endl;
cout << POLYNOMIAL(0, 5) / POLYNOMIAL(2, 6, 3, 10) << endl;
cout << POLYNOMIAL(0, 5) % POLYNOMIAL(2, 6, 3, 10) << endl;
cout << POLYNOMIAL(3, 7, 10, 0, 9) / POLYNOMIAL(3, -10, -2, -1, 3) << endl;
cout << POLYNOMIAL(3, 7, 10, 0, 9) % POLYNOMIAL(3, -10, -2, -1, 3) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(3, -2, 5, 5, -6) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(3, -2, 5, 5, -6) << endl;
cout << POLYNOMIAL(1, -2, 6) / POLYNOMIAL(2, 3, 8, -9) << endl;
cout << POLYNOMIAL(1, -2, 6) % POLYNOMIAL(2, 3, 8, -9) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(1, -10, 1) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(1, -10, 1) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(3, -4, -5, -8, -8) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(3, -4, -5, -8, -8) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(1, -7, 8) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(1, -7, 8) << endl;
cout << POLYNOMIAL(1, 8, 3) / POLYNOMIAL(1, -8, -5) << endl;
cout << POLYNOMIAL(1, 8, 3) % POLYNOMIAL(1, -8, -5) << endl;
cout << POLYNOMIAL(3, 2, 6, -8, 8) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(3, 2, 6, -8, 8) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(1, 9, 7) / POLYNOMIAL(3, -6, -7, -1, -8) << endl;
cout << POLYNOMIAL(1, 9, 7) % POLYNOMIAL(3, -6, -7, -1, -8) << endl;
cout << POLYNOMIAL(2, -8, 0, -8) / POLYNOMIAL(3, 2, -3, 2, -7) << endl;
cout << POLYNOMIAL(2, -8, 0, -8) % POLYNOMIAL(3, 2, -3, 2, -7) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(3, -7, 5, -5, -1) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(3, -7, 5, -5, -1) << endl;
cout << POLYNOMIAL(3, -3, -8, 2, -6) / POLYNOMIAL(1, 7, 0) << endl;
cout << POLYNOMIAL(3, -3, -8, 2, -6) % POLYNOMIAL(1, 7, 0) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(3, 2, 6, 9, -3) / POLYNOMIAL(2, 3, -1, 7) << endl;
cout << POLYNOMIAL(3, 2, 6, 9, -3) % POLYNOMIAL(2, 3, -1, 7) << endl;
cout << POLYNOMIAL(3, 1, -4, 1, -4) / POLYNOMIAL(1, -3, -8) << endl;
cout << POLYNOMIAL(3, 1, -4, 1, -4) % POLYNOMIAL(1, -3, -8) << endl;
cout << POLYNOMIAL(1, 0, 0) / POLYNOMIAL(3, 9, -10, 10, -8) << endl;
cout << POLYNOMIAL(1, 0, 0) % POLYNOMIAL(3, 9, -10, 10, -8) << endl;
cout << POLYNOMIAL(3, -9, 7, 10, -3) / POLYNOMIAL(3, -1, 10, 5, 2) << endl;
cout << POLYNOMIAL(3, -9, 7, 10, -3) % POLYNOMIAL(3, -1, 10, 5, 2) << endl;
cout << POLYNOMIAL(3, 8, -8, -10, 1) / POLYNOMIAL(3, 8, -2, 4, 7) << endl;
cout << POLYNOMIAL(3, 8, -8, -10, 1) % POLYNOMIAL(3, 8, -2, 4, 7) << endl;
cout << POLYNOMIAL(3, -10, -6, -5, 5) / POLYNOMIAL(1, 10, 0) << endl;
cout << POLYNOMIAL(3, -10, -6, -5, 5) % POLYNOMIAL(1, 10, 0) << endl;
cout << POLYNOMIAL(3, 1, 3, 9, 7) / POLYNOMIAL(2, 8, -2, 10) << endl;
cout << POLYNOMIAL(3, 1, 3, 9, 7) % POLYNOMIAL(2, 8, -2, 10) << endl;
cout << POLYNOMIAL(3, -7, 9, 3, -2) / POLYNOMIAL(1, 8, -5) << endl;
cout << POLYNOMIAL(3, -7, 9, 3, -2) % POLYNOMIAL(1, 8, -5) << endl;
cout << POLYNOMIAL(3, -5, 9, 9, -1) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(3, -5, 9, 9, -1) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(0, 2) / POLYNOMIAL(3, 2, -3, 4, 8) << endl;
cout << POLYNOMIAL(0, 2) % POLYNOMIAL(3, 2, -3, 4, 8) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(2, 1, 1, -9) / POLYNOMIAL(2, -3, 4, -5) << endl;
cout << POLYNOMIAL(2, 1, 1, -9) % POLYNOMIAL(2, -3, 4, -5) << endl;
cout << POLYNOMIAL(1, 10, -8) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(1, 10, -8) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(3, 0, 3, -7, 4) / POLYNOMIAL(1, -5, 1) << endl;
cout << POLYNOMIAL(3, 0, 3, -7, 4) % POLYNOMIAL(1, -5, 1) << endl;
cout << POLYNOMIAL(1, 8, 1) / POLYNOMIAL(1, -8, 7) << endl;
cout << POLYNOMIAL(1, 8, 1) % POLYNOMIAL(1, -8, 7) << endl;
cout << POLYNOMIAL(3, 2, 8, -5, 7) / POLYNOMIAL(2, 7, -8, -10) << endl;
cout << POLYNOMIAL(3, 2, 8, -5, 7) % POLYNOMIAL(2, 7, -8, -10) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(1, -1, 4) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(1, -1, 4) << endl;
cout << POLYNOMIAL(3, 5, 6, -3, -1) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(3, 5, 6, -3, -1) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, -8, -10, -1) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(2, -8, -10, -1) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(1, -4, 5) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(1, -4, 5) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(3, -4, -3, -8, 9) / POLYNOMIAL(1, 2, -3) << endl;
cout << POLYNOMIAL(3, -4, -3, -8, 9) % POLYNOMIAL(1, 2, -3) << endl;
cout << POLYNOMIAL(3, 2, -4, 2, 8) / POLYNOMIAL(2, -2, 5, -5) << endl;
cout << POLYNOMIAL(3, 2, -4, 2, 8) % POLYNOMIAL(2, -2, 5, -5) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(3, 5, 7, 3, 1) / POLYNOMIAL(3, -9, 7, -2, -7) << endl;
cout << POLYNOMIAL(3, 5, 7, 3, 1) % POLYNOMIAL(3, -9, 7, -2, -7) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(3, -6, -3, -8, -7) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(3, -6, -3, -8, -7) << endl;
cout << POLYNOMIAL(1, -1, 1) / POLYNOMIAL(3, -8, -10, 7, -10) << endl;
cout << POLYNOMIAL(1, -1, 1) % POLYNOMIAL(3, -8, -10, 7, -10) << endl;
cout << POLYNOMIAL(3, 5, -6, 7, -1) / POLYNOMIAL(2, 3, -9, 8) << endl;
cout << POLYNOMIAL(3, 5, -6, 7, -1) % POLYNOMIAL(2, 3, -9, 8) << endl;
cout << POLYNOMIAL(2, -3, -3, -9) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, -3, -3, -9) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(1, -2, -3) / POLYNOMIAL(1, 5, -6) << endl;
cout << POLYNOMIAL(1, -2, -3) % POLYNOMIAL(1, 5, -6) << endl;
cout << POLYNOMIAL(1, 0, -6) / POLYNOMIAL(1, -5, -5) << endl;
cout << POLYNOMIAL(1, 0, -6) % POLYNOMIAL(1, -5, -5) << endl;
cout << POLYNOMIAL(3, -1, -1, -6, -2) / POLYNOMIAL(2, 6, -6, 9) << endl;
cout << POLYNOMIAL(3, -1, -1, -6, -2) % POLYNOMIAL(2, 6, -6, 9) << endl;
cout << POLYNOMIAL(1, 6, -8) / POLYNOMIAL(3, 4, 4, 6, 4) << endl;
cout << POLYNOMIAL(1, 6, -8) % POLYNOMIAL(3, 4, 4, 6, 4) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(1, 6, 9) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(1, 6, 9) << endl;
cout << POLYNOMIAL(3, -4, 5, -3, -7) / POLYNOMIAL(3, 5, 8, -1, -2) << endl;
cout << POLYNOMIAL(3, -4, 5, -3, -7) % POLYNOMIAL(3, 5, 8, -1, -2) << endl;
cout << POLYNOMIAL(2, 8, -10, 3) / POLYNOMIAL(1, 4, -9) << endl;
cout << POLYNOMIAL(2, 8, -10, 3) % POLYNOMIAL(1, 4, -9) << endl;
cout << POLYNOMIAL(2, -6, 8, -1) / POLYNOMIAL(2, -6, 1, 4) << endl;
cout << POLYNOMIAL(2, -6, 8, -1) % POLYNOMIAL(2, -6, 1, 4) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(3, -2, -10, -8, -7) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(3, -2, -10, -8, -7) << endl;
cout << POLYNOMIAL(1, -10, -1) / POLYNOMIAL(1, 6, -9) << endl;
cout << POLYNOMIAL(1, -10, -1) % POLYNOMIAL(1, 6, -9) << endl;
cout << POLYNOMIAL(1, 0, -10) / POLYNOMIAL(3, -10, 3, 5, -6) << endl;
cout << POLYNOMIAL(1, 0, -10) % POLYNOMIAL(3, -10, 3, 5, -6) << endl;
cout << POLYNOMIAL(2, 8, 3, 9) / POLYNOMIAL(1, -6, -1) << endl;
cout << POLYNOMIAL(2, 8, 3, 9) % POLYNOMIAL(1, -6, -1) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(2, -9, -3, 9) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(2, -9, -3, 9) << endl;
cout << POLYNOMIAL(3, 7, 7, -1, 9) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(3, 7, 7, -1, 9) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(2, 9, -10, 3) / POLYNOMIAL(2, -2, 1, -4) << endl;
cout << POLYNOMIAL(2, 9, -10, 3) % POLYNOMIAL(2, -2, 1, -4) << endl;
cout << POLYNOMIAL(3, -1, 10, 9, 0) / POLYNOMIAL(3, -6, 6, -1, 5) << endl;
cout << POLYNOMIAL(3, -1, 10, 9, 0) % POLYNOMIAL(3, -6, 6, -1, 5) << endl;
cout << POLYNOMIAL(0, 5) / POLYNOMIAL(1, -3, -5) << endl;
cout << POLYNOMIAL(0, 5) % POLYNOMIAL(1, -3, -5) << endl;
cout << POLYNOMIAL(2, -5, -5, -1) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(2, -5, -5, -1) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(3, -1, 2, 1, 2) / POLYNOMIAL(2, 3, -2, -2) << endl;
cout << POLYNOMIAL(3, -1, 2, 1, 2) % POLYNOMIAL(2, 3, -2, -2) << endl;
cout << POLYNOMIAL(3, 5, 1, -10, 4) / POLYNOMIAL(3, -3, -3, 5, 7) << endl;
cout << POLYNOMIAL(3, 5, 1, -10, 4) % POLYNOMIAL(3, -3, -3, 5, 7) << endl;
cout << POLYNOMIAL(2, 2, -2, 8) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(2, 2, -2, 8) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(2, 7, 8, -2) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(2, 7, 8, -2) << endl;
cout << POLYNOMIAL(3, -3, 8, -1, -2) / POLYNOMIAL(3, -1, 3, 6, 7) << endl;
cout << POLYNOMIAL(3, -3, 8, -1, -2) % POLYNOMIAL(3, -1, 3, 6, 7) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(2, 9, 6, -5) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(2, 9, 6, -5) << endl;
cout << POLYNOMIAL(3, 6, 7, -8, -3) / POLYNOMIAL(3, 8, -2, 5, 3) << endl;
cout << POLYNOMIAL(3, 6, 7, -8, -3) % POLYNOMIAL(3, 8, -2, 5, 3) << endl;
cout << POLYNOMIAL(3, -1, -7, 6, -1) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(3, -1, -7, 6, -1) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(1, -8, -10) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(1, -8, -10) << endl;
cout << POLYNOMIAL(1, 0, 5) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(1, 0, 5) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(1, -4, -10) / POLYNOMIAL(3, -2, 8, -8, -2) << endl;
cout << POLYNOMIAL(1, -4, -10) % POLYNOMIAL(3, -2, 8, -8, -2) << endl;
cout << POLYNOMIAL(3, -2, -5, -8, -3) / POLYNOMIAL(2, -10, -9, 3) << endl;
cout << POLYNOMIAL(3, -2, -5, -8, -3) % POLYNOMIAL(2, -10, -9, 3) << endl;
cout << POLYNOMIAL(2, -9, -8, 2) / POLYNOMIAL(1, -6, -5) << endl;
cout << POLYNOMIAL(2, -9, -8, 2) % POLYNOMIAL(1, -6, -5) << endl;
cout << POLYNOMIAL(3, -8, 2, -6, 3) / POLYNOMIAL(3, 2, -7, -10, -3) << endl;
cout << POLYNOMIAL(3, -8, 2, -6, 3) % POLYNOMIAL(3, 2, -7, -10, -3) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(1, -7, 2) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(1, -7, 2) << endl;
cout << POLYNOMIAL(3, -9, -3, -4, -9) / POLYNOMIAL(2, 5, 2, -7) << endl;
cout << POLYNOMIAL(3, -9, -3, -4, -9) % POLYNOMIAL(2, 5, 2, -7) << endl;
cout << POLYNOMIAL(3, 8, -5, 5, -10) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(3, 8, -5, 5, -10) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(2, 10, 1, 6) / POLYNOMIAL(2, -8, -8, -4) << endl;
cout << POLYNOMIAL(2, 10, 1, 6) % POLYNOMIAL(2, -8, -8, -4) << endl;
cout << POLYNOMIAL(2, 0, 8, -7) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, 0, 8, -7) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, -2, 9, -10) / POLYNOMIAL(3, 7, -7, 6, -2) << endl;
cout << POLYNOMIAL(2, -2, 9, -10) % POLYNOMIAL(3, 7, -7, 6, -2) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(3, -8, -2, -2, 4) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(3, -8, -2, -2, 4) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(2, -1, 3, 8) / POLYNOMIAL(2, -6, -7, 7) << endl;
cout << POLYNOMIAL(2, -1, 3, 8) % POLYNOMIAL(2, -6, -7, 7) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(2, 3, -7, 1) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(2, 3, -7, 1) << endl;
cout << POLYNOMIAL(3, 8, 2, -10, 7) / POLYNOMIAL(2, 4, -7, -9) << endl;
cout << POLYNOMIAL(3, 8, 2, -10, 7) % POLYNOMIAL(2, 4, -7, -9) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(2, 4, 6, 4) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(2, 4, 6, 4) << endl;
cout << POLYNOMIAL(3, 8, -5, 3, -10) / POLYNOMIAL(3, -4, 10, -4, -6) << endl;
cout << POLYNOMIAL(3, 8, -5, 3, -10) % POLYNOMIAL(3, -4, 10, -4, -6) << endl;
cout << POLYNOMIAL(3, -8, 6, -2, 2) / POLYNOMIAL(3, -3, -6, 9, -4) << endl;
cout << POLYNOMIAL(3, -8, 6, -2, 2) % POLYNOMIAL(3, -3, -6, 9, -4) << endl;
cout << POLYNOMIAL(1, 10, -2) / POLYNOMIAL(1, 10, -4) << endl;
cout << POLYNOMIAL(1, 10, -2) % POLYNOMIAL(1, 10, -4) << endl;
cout << POLYNOMIAL(3, 6, -2, -5, -9) / POLYNOMIAL(1, 3, -10) << endl;
cout << POLYNOMIAL(3, 6, -2, -5, -9) % POLYNOMIAL(1, 3, -10) << endl;
cout << POLYNOMIAL(2, 1, -9, -7) / POLYNOMIAL(1, -2, 0) << endl;
cout << POLYNOMIAL(2, 1, -9, -7) % POLYNOMIAL(1, -2, 0) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(1, 3, -1) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(1, 3, -1) << endl;
cout << POLYNOMIAL(2, 5, -7, 8) / POLYNOMIAL(1, -4, 6) << endl;
cout << POLYNOMIAL(2, 5, -7, 8) % POLYNOMIAL(1, -4, 6) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(1, -10, -6) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(1, -10, -6) << endl;
cout << POLYNOMIAL(2, 1, -7, -10) / POLYNOMIAL(1, -3, 3) << endl;
cout << POLYNOMIAL(2, 1, -7, -10) % POLYNOMIAL(1, -3, 3) << endl;
cout << POLYNOMIAL(0, 6) / POLYNOMIAL(3, 3, 2, 8, -3) << endl;
cout << POLYNOMIAL(0, 6) % POLYNOMIAL(3, 3, 2, 8, -3) << endl;
cout << POLYNOMIAL(3, 3, 3, -5, 8) / POLYNOMIAL(2, -2, -9, 7) << endl;
cout << POLYNOMIAL(3, 3, 3, -5, 8) % POLYNOMIAL(2, -2, -9, 7) << endl;
cout << POLYNOMIAL(3, -5, -9, 2, 10) / POLYNOMIAL(1, 9, 7) << endl;
cout << POLYNOMIAL(3, -5, -9, 2, 10) % POLYNOMIAL(1, 9, 7) << endl;
cout << POLYNOMIAL(3, -3, 3, 6, 7) / POLYNOMIAL(1, 7, -1) << endl;
cout << POLYNOMIAL(3, -3, 3, 6, 7) % POLYNOMIAL(1, 7, -1) << endl;
cout << POLYNOMIAL(1, -9, 4) / POLYNOMIAL(2, 4, -7, -10) << endl;
cout << POLYNOMIAL(1, -9, 4) % POLYNOMIAL(2, 4, -7, -10) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(1, -3, 9) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(1, -3, 9) << endl;
cout << POLYNOMIAL(2, -2, -10, -2) / POLYNOMIAL(2, 6, -1, -6) << endl;
cout << POLYNOMIAL(2, -2, -10, -2) % POLYNOMIAL(2, 6, -1, -6) << endl;
cout << POLYNOMIAL(2, 8, 4, 9) / POLYNOMIAL(1, -4, -8) << endl;
cout << POLYNOMIAL(2, 8, 4, 9) % POLYNOMIAL(1, -4, -8) << endl;
cout << POLYNOMIAL(1, 5, 6) / POLYNOMIAL(2, 0, 3, -2) << endl;
cout << POLYNOMIAL(1, 5, 6) % POLYNOMIAL(2, 0, 3, -2) << endl;
cout << POLYNOMIAL(2, 2, -1, -6) / POLYNOMIAL(3, -10, -3, 5, 4) << endl;
cout << POLYNOMIAL(2, 2, -1, -6) % POLYNOMIAL(3, -10, -3, 5, 4) << endl;
cout << POLYNOMIAL(1, 2, -9) / POLYNOMIAL(2, -1, 7, 6) << endl;
cout << POLYNOMIAL(1, 2, -9) % POLYNOMIAL(2, -1, 7, 6) << endl;
cout << POLYNOMIAL(3, 2, -1, 6, -6) / POLYNOMIAL(1, 10, 6) << endl;
cout << POLYNOMIAL(3, 2, -1, 6, -6) % POLYNOMIAL(1, 10, 6) << endl;
cout << POLYNOMIAL(3, 3, 5, 4, 5) / POLYNOMIAL(3, -4, -10, 1, -10) << endl;
cout << POLYNOMIAL(3, 3, 5, 4, 5) % POLYNOMIAL(3, -4, -10, 1, -10) << endl;
cout << POLYNOMIAL(1, -6, 6) / POLYNOMIAL(2, 0, -5, 0) << endl;
cout << POLYNOMIAL(1, -6, 6) % POLYNOMIAL(2, 0, -5, 0) << endl;
cout << POLYNOMIAL(3, -1, 0, 3, 0) / POLYNOMIAL(3, 5, -5, 2, -8) << endl;
cout << POLYNOMIAL(3, -1, 0, 3, 0) % POLYNOMIAL(3, 5, -5, 2, -8) << endl;
cout << POLYNOMIAL(1, 3, -4) / POLYNOMIAL(1, -3, -9) << endl;
cout << POLYNOMIAL(1, 3, -4) % POLYNOMIAL(1, -3, -9) << endl;
cout << POLYNOMIAL(3, -5, -7, 7, 4) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(3, -5, -7, 7, 4) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, -8, 9) / POLYNOMIAL(3, 2, 3, -9, -9) << endl;
cout << POLYNOMIAL(1, -8, 9) % POLYNOMIAL(3, 2, 3, -9, -9) << endl;
cout << POLYNOMIAL(2, -1, -9, -5) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(2, -1, -9, -5) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(3, -3, 0, -9, -4) / POLYNOMIAL(2, -10, -8, -10) << endl;
cout << POLYNOMIAL(3, -3, 0, -9, -4) % POLYNOMIAL(2, -10, -8, -10) << endl;
cout << POLYNOMIAL(1, 7, -7) / POLYNOMIAL(3, -1, -1, 3, -9) << endl;
cout << POLYNOMIAL(1, 7, -7) % POLYNOMIAL(3, -1, -1, 3, -9) << endl;
cout << POLYNOMIAL(3, -1, -6, -10, -5) / POLYNOMIAL(2, 0, -2, -6) << endl;
cout << POLYNOMIAL(3, -1, -6, -10, -5) % POLYNOMIAL(2, 0, -2, -6) << endl;
cout << POLYNOMIAL(0, 5) / POLYNOMIAL(2, 8, 1, 4) << endl;
cout << POLYNOMIAL(0, 5) % POLYNOMIAL(2, 8, 1, 4) << endl;
cout << POLYNOMIAL(1, 6, -5) / POLYNOMIAL(2, -10, 3, -1) << endl;
cout << POLYNOMIAL(1, 6, -5) % POLYNOMIAL(2, -10, 3, -1) << endl;
cout << POLYNOMIAL(1, 1, -7) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(1, 1, -7) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(1, -9, -5) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(1, -9, -5) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(1, 2, 8) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(1, 2, 8) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(0, 2) / POLYNOMIAL(3, -2, 4, 4, 9) << endl;
cout << POLYNOMIAL(0, 2) % POLYNOMIAL(3, -2, 4, 4, 9) << endl;
cout << POLYNOMIAL(3, 4, 6, 8, -8) / POLYNOMIAL(2, 0, -9, 5) << endl;
cout << POLYNOMIAL(3, 4, 6, 8, -8) % POLYNOMIAL(2, 0, -9, 5) << endl;
cout << POLYNOMIAL(1, -5, 6) / POLYNOMIAL(3, 1, 2, -9, 0) << endl;
cout << POLYNOMIAL(1, -5, 6) % POLYNOMIAL(3, 1, 2, -9, 0) << endl;
cout << POLYNOMIAL(2, 2, -3, -10) / POLYNOMIAL(1, -1, -1) << endl;
cout << POLYNOMIAL(2, 2, -3, -10) % POLYNOMIAL(1, -1, -1) << endl;
cout << POLYNOMIAL(1, 6, 1) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(1, 6, 1) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(2, -10, 0, 5) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(2, -10, 0, 5) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(2, -3, 8, 0) / POLYNOMIAL(1, 8, 10) << endl;
cout << POLYNOMIAL(2, -3, 8, 0) % POLYNOMIAL(1, 8, 10) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(3, -8, 9, -6, -9) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(3, -8, 9, -6, -9) << endl;
cout << POLYNOMIAL(3, -5, 6, -2, 2) / POLYNOMIAL(1, 8, -7) << endl;
cout << POLYNOMIAL(3, -5, 6, -2, 2) % POLYNOMIAL(1, 8, -7) << endl;
cout << POLYNOMIAL(1, -3, 1) / POLYNOMIAL(2, -10, -4, 7) << endl;
cout << POLYNOMIAL(1, -3, 1) % POLYNOMIAL(2, -10, -4, 7) << endl;
cout << POLYNOMIAL(1, 4, -4) / POLYNOMIAL(3, 9, -7, -3, -8) << endl;
cout << POLYNOMIAL(1, 4, -4) % POLYNOMIAL(3, 9, -7, -3, -8) << endl;
cout << POLYNOMIAL(3, -3, 0, 9, 6) / POLYNOMIAL(2, -10, -6, 5) << endl;
cout << POLYNOMIAL(3, -3, 0, 9, 6) % POLYNOMIAL(2, -10, -6, 5) << endl;
cout << POLYNOMIAL(1, 2, -10) / POLYNOMIAL(1, -10, 5) << endl;
cout << POLYNOMIAL(1, 2, -10) % POLYNOMIAL(1, -10, 5) << endl;
cout << POLYNOMIAL(0, 6) / POLYNOMIAL(1, 8, 6) << endl;
cout << POLYNOMIAL(0, 6) % POLYNOMIAL(1, 8, 6) << endl;
cout << POLYNOMIAL(1, -8, 1) / POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(1, -8, 1) % POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(1, 10, -8) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(1, 10, -8) << endl;
cout << POLYNOMIAL(3, 5, 4, 2, 6) / POLYNOMIAL(2, 4, -6, 2) << endl;
cout << POLYNOMIAL(3, 5, 4, 2, 6) % POLYNOMIAL(2, 4, -6, 2) << endl;
cout << POLYNOMIAL(1, 7, 7) / POLYNOMIAL(2, 7, 1, -3) << endl;
cout << POLYNOMIAL(1, 7, 7) % POLYNOMIAL(2, 7, 1, -3) << endl;
cout << POLYNOMIAL(2, -1, -7, 10) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(2, -1, -7, 10) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(3, 9, 10, 0, -4) / POLYNOMIAL(2, -6, 9, -7) << endl;
cout << POLYNOMIAL(3, 9, 10, 0, -4) % POLYNOMIAL(2, -6, 9, -7) << endl;
cout << POLYNOMIAL(3, -4, -6, 9, -8) / POLYNOMIAL(2, 5, -5, -2) << endl;
cout << POLYNOMIAL(3, -4, -6, 9, -8) % POLYNOMIAL(2, 5, -5, -2) << endl;
cout << POLYNOMIAL(1, 2, 1) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(1, 2, 1) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(3, 8, -10, -7, 1) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, 8, -10, -7, 1) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(1, 8, -5) / POLYNOMIAL(3, 9, 6, 0, 4) << endl;
cout << POLYNOMIAL(1, 8, -5) % POLYNOMIAL(3, 9, 6, 0, 4) << endl;
cout << POLYNOMIAL(3, 2, -1, 2, -7) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(3, 2, -1, 2, -7) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(0, 2) / POLYNOMIAL(2, 9, -8, 3) << endl;
cout << POLYNOMIAL(0, 2) % POLYNOMIAL(2, 9, -8, 3) << endl;
cout << POLYNOMIAL(3, 8, -7, -4, -1) / POLYNOMIAL(3, -8, -5, -10, -5) << endl;
cout << POLYNOMIAL(3, 8, -7, -4, -1) % POLYNOMIAL(3, -8, -5, -10, -5) << endl;
cout << POLYNOMIAL(1, -7, -4) / POLYNOMIAL(3, 2, -2, -10, 0) << endl;
cout << POLYNOMIAL(1, -7, -4) % POLYNOMIAL(3, 2, -2, -10, 0) << endl;
cout << POLYNOMIAL(1, 5, -6) / POLYNOMIAL(3, 6, -2, 6, 9) << endl;
cout << POLYNOMIAL(1, 5, -6) % POLYNOMIAL(3, 6, -2, 6, 9) << endl;
cout << POLYNOMIAL(2, -4, -1, 5) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(2, -4, -1, 5) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(2, 6, 4, 0) / POLYNOMIAL(3, -4, -10, 6, 2) << endl;
cout << POLYNOMIAL(2, 6, 4, 0) % POLYNOMIAL(3, -4, -10, 6, 2) << endl;
cout << POLYNOMIAL(1, -5, -1) / POLYNOMIAL(1, 0, 3) << endl;
cout << POLYNOMIAL(1, -5, -1) % POLYNOMIAL(1, 0, 3) << endl;
cout << POLYNOMIAL(1, 8, 7) / POLYNOMIAL(3, 3, 2, 0, 6) << endl;
cout << POLYNOMIAL(1, 8, 7) % POLYNOMIAL(3, 3, 2, 0, 6) << endl;
cout << POLYNOMIAL(1, 9, 3) / POLYNOMIAL(2, -8, 0, 10) << endl;
cout << POLYNOMIAL(1, 9, 3) % POLYNOMIAL(2, -8, 0, 10) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(3, 1, 10, -2, 2) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(3, 1, 10, -2, 2) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(1, 6, -8) / POLYNOMIAL(2, -1, -7, -9) << endl;
cout << POLYNOMIAL(1, 6, -8) % POLYNOMIAL(2, -1, -7, -9) << endl;
cout << POLYNOMIAL(3, -3, -10, 4, 1) / POLYNOMIAL(2, 5, 1, 9) << endl;
cout << POLYNOMIAL(3, -3, -10, 4, 1) % POLYNOMIAL(2, 5, 1, 9) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(2, -5, 6, -7) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(2, -5, 6, -7) << endl;
cout << POLYNOMIAL(1, 6, -2) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(1, 6, -2) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(3, 3, -3, 1, 7) / POLYNOMIAL(1, -6, -9) << endl;
cout << POLYNOMIAL(3, 3, -3, 1, 7) % POLYNOMIAL(1, -6, -9) << endl;
cout << POLYNOMIAL(3, 5, 1, -10, -2) / POLYNOMIAL(3, 8, -7, 10, -8) << endl;
cout << POLYNOMIAL(3, 5, 1, -10, -2) % POLYNOMIAL(3, 8, -7, 10, -8) << endl;
cout << POLYNOMIAL(3, -9, 0, 2, 3) / POLYNOMIAL(3, -8, 3, -3, -8) << endl;
cout << POLYNOMIAL(3, -9, 0, 2, 3) % POLYNOMIAL(3, -8, 3, -3, -8) << endl;
cout << POLYNOMIAL(0, 2) / POLYNOMIAL(2, 7, -9, 9) << endl;
cout << POLYNOMIAL(0, 2) % POLYNOMIAL(2, 7, -9, 9) << endl;
cout << POLYNOMIAL(3, 2, -10, -2, 5) / POLYNOMIAL(3, -9, 1, 10, 9) << endl;
cout << POLYNOMIAL(3, 2, -10, -2, 5) % POLYNOMIAL(3, -9, 1, 10, 9) << endl;
cout << POLYNOMIAL(2, -2, 5, -8) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(2, -2, 5, -8) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(3, -6, 4, -9, 7) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(3, -6, 4, -9, 7) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(3, -9, 1, -3, 5) / POLYNOMIAL(1, -7, -9) << endl;
cout << POLYNOMIAL(3, -9, 1, -3, 5) % POLYNOMIAL(1, -7, -9) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(1, 9, 3) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(1, 9, 3) << endl;
cout << POLYNOMIAL(1, 10, 2) / POLYNOMIAL(3, -3, 3, 6, 3) << endl;
cout << POLYNOMIAL(1, 10, 2) % POLYNOMIAL(3, -3, 3, 6, 3) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(2, 3, 3, 9) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(2, 3, 3, 9) << endl;
cout << POLYNOMIAL(1, 1, -8) / POLYNOMIAL(3, -10, -8, -7, -2) << endl;
cout << POLYNOMIAL(1, 1, -8) % POLYNOMIAL(3, -10, -8, -7, -2) << endl;
cout << POLYNOMIAL(1, 10, -10) / POLYNOMIAL(3, 3, 8, 0, 5) << endl;
cout << POLYNOMIAL(1, 10, -10) % POLYNOMIAL(3, 3, 8, 0, 5) << endl;
cout << POLYNOMIAL(3, 5, 7, -8, -7) / POLYNOMIAL(1, -5, 2) << endl;
cout << POLYNOMIAL(3, 5, 7, -8, -7) % POLYNOMIAL(1, -5, 2) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(2, -2, 1, -8) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(2, -2, 1, -8) << endl;
cout << POLYNOMIAL(3, 10, -3, -8, 0) / POLYNOMIAL(2, 5, -8, -4) << endl;
cout << POLYNOMIAL(3, 10, -3, -8, 0) % POLYNOMIAL(2, 5, -8, -4) << endl;
cout << POLYNOMIAL(1, -8, -6) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(1, -8, -6) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(2, 4, -4, -7) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(2, 4, -4, -7) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(1, 2, 4) / POLYNOMIAL(2, 10, 6, -6) << endl;
cout << POLYNOMIAL(1, 2, 4) % POLYNOMIAL(2, 10, 6, -6) << endl;
cout << POLYNOMIAL(3, -2, -6, -8, 6) / POLYNOMIAL(3, -2, -4, -4, 2) << endl;
cout << POLYNOMIAL(3, -2, -6, -8, 6) % POLYNOMIAL(3, -2, -4, -4, 2) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(3, 0, -3, 8, 6) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(3, 0, -3, 8, 6) << endl;
cout << POLYNOMIAL(1, 1, -10) / POLYNOMIAL(3, 3, 8, 0, 3) << endl;
cout << POLYNOMIAL(1, 1, -10) % POLYNOMIAL(3, 3, 8, 0, 3) << endl;
cout << POLYNOMIAL(3, 1, 4, 7, 5) / POLYNOMIAL(2, -10, -3, 7) << endl;
cout << POLYNOMIAL(3, 1, 4, 7, 5) % POLYNOMIAL(2, -10, -3, 7) << endl;
cout << POLYNOMIAL(3, 6, 8, -8, 9) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(3, 6, 8, -8, 9) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(1, -4, 9) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(1, -4, 9) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(1, 4, -2) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(1, 4, -2) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, 9, 10, 6) / POLYNOMIAL(2, -7, -4, -10) << endl;
cout << POLYNOMIAL(2, 9, 10, 6) % POLYNOMIAL(2, -7, -4, -10) << endl;
cout << POLYNOMIAL(2, 7, -7, 2) / POLYNOMIAL(1, 3, 2) << endl;
cout << POLYNOMIAL(2, 7, -7, 2) % POLYNOMIAL(1, 3, 2) << endl;
cout << POLYNOMIAL(1, -8, -2) / POLYNOMIAL(1, -1, -9) << endl;
cout << POLYNOMIAL(1, -8, -2) % POLYNOMIAL(1, -1, -9) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(1, -10, -4) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(1, -10, -4) << endl;
cout << POLYNOMIAL(2, -1, 1, 7) / POLYNOMIAL(2, 6, 0, -5) << endl;
cout << POLYNOMIAL(2, -1, 1, 7) % POLYNOMIAL(2, 6, 0, -5) << endl;
cout << POLYNOMIAL(2, -1, -1, 3) / POLYNOMIAL(1, 2, 4) << endl;
cout << POLYNOMIAL(2, -1, -1, 3) % POLYNOMIAL(1, 2, 4) << endl;
cout << POLYNOMIAL(1, 2, -4) / POLYNOMIAL(2, 8, 1, -3) << endl;
cout << POLYNOMIAL(1, 2, -4) % POLYNOMIAL(2, 8, 1, -3) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(1, 2, 10) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(1, 2, 10) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(2, 4, 0, 4) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(2, 4, 0, 4) << endl;
cout << POLYNOMIAL(1, -2, -6) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(1, -2, -6) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(3, 6, 3, 5, -9) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(3, 6, 3, 5, -9) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(1, 1, 1) / POLYNOMIAL(2, 6, -7, -9) << endl;
cout << POLYNOMIAL(1, 1, 1) % POLYNOMIAL(2, 6, -7, -9) << endl;
cout << POLYNOMIAL(3, -4, -5, 5, -1) / POLYNOMIAL(1, 9, -5) << endl;
cout << POLYNOMIAL(3, -4, -5, 5, -1) % POLYNOMIAL(1, 9, -5) << endl;
cout << POLYNOMIAL(1, 4, 7) / POLYNOMIAL(1, 2, 5) << endl;
cout << POLYNOMIAL(1, 4, 7) % POLYNOMIAL(1, 2, 5) << endl;
cout << POLYNOMIAL(3, 6, 10, 7, -9) / POLYNOMIAL(2, 3, 4, -1) << endl;
cout << POLYNOMIAL(3, 6, 10, 7, -9) % POLYNOMIAL(2, 3, 4, -1) << endl;
cout << POLYNOMIAL(2, -8, -3, -5) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, -8, -3, -5) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(1, -2, -4) / POLYNOMIAL(2, -7, 4, -1) << endl;
cout << POLYNOMIAL(1, -2, -4) % POLYNOMIAL(2, -7, 4, -1) << endl;
cout << POLYNOMIAL(3, -2, 4, 6, -7) / POLYNOMIAL(2, -5, 10, 4) << endl;
cout << POLYNOMIAL(3, -2, 4, 6, -7) % POLYNOMIAL(2, -5, 10, 4) << endl;
cout << POLYNOMIAL(1, -4, 0) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(1, -4, 0) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(1, 10, 3) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(1, 10, 3) << endl;
cout << POLYNOMIAL(1, -10, 2) / POLYNOMIAL(2, 2, 4, 3) << endl;
cout << POLYNOMIAL(1, -10, 2) % POLYNOMIAL(2, 2, 4, 3) << endl;
cout << POLYNOMIAL(3, 0, -1, 9, -5) / POLYNOMIAL(1, -6, 3) << endl;
cout << POLYNOMIAL(3, 0, -1, 9, -5) % POLYNOMIAL(1, -6, 3) << endl;
cout << POLYNOMIAL(1, 0, 1) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(1, 0, 1) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(3, 0, -9, -9, 4) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(3, 0, -9, -9, 4) << endl;
cout << POLYNOMIAL(2, -3, 10, 3) / POLYNOMIAL(1, 5, 4) << endl;
cout << POLYNOMIAL(2, -3, 10, 3) % POLYNOMIAL(1, 5, 4) << endl;
cout << POLYNOMIAL(1, -8, -1) / POLYNOMIAL(1, 4, -3) << endl;
cout << POLYNOMIAL(1, -8, -1) % POLYNOMIAL(1, 4, -3) << endl;
cout << POLYNOMIAL(3, -9, 0, 2, 1) / POLYNOMIAL(3, -9, 9, 9, 10) << endl;
cout << POLYNOMIAL(3, -9, 0, 2, 1) % POLYNOMIAL(3, -9, 9, 9, 10) << endl;
cout << POLYNOMIAL(1, 6, -8) / POLYNOMIAL(1, 0, 10) << endl;
cout << POLYNOMIAL(1, 6, -8) % POLYNOMIAL(1, 0, 10) << endl;
cout << POLYNOMIAL(2, 10, 3, 9) / POLYNOMIAL(2, 6, 9, -6) << endl;
cout << POLYNOMIAL(2, 10, 3, 9) % POLYNOMIAL(2, 6, 9, -6) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(2, -6, -7, 7) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(2, -6, -7, 7) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(3, 7, 1, 3, -9) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(3, 7, 1, 3, -9) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(3, 0, 3, -8, 3) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(3, 0, 3, -8, 3) << endl;
cout << POLYNOMIAL(1, -4, 1) / POLYNOMIAL(3, 6, 3, 6, 7) << endl;
cout << POLYNOMIAL(1, -4, 1) % POLYNOMIAL(3, 6, 3, 6, 7) << endl;
cout << POLYNOMIAL(3, 9, -5, -4, -4) / POLYNOMIAL(3, 2, -2, 7, 4) << endl;
cout << POLYNOMIAL(3, 9, -5, -4, -4) % POLYNOMIAL(3, 2, -2, 7, 4) << endl;
cout << POLYNOMIAL(1, -1, 10) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(1, -1, 10) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(1, 2, -5) / POLYNOMIAL(3, -1, 8, 2, 1) << endl;
cout << POLYNOMIAL(1, 2, -5) % POLYNOMIAL(3, -1, 8, 2, 1) << endl;
cout << POLYNOMIAL(1, -3, -4) / POLYNOMIAL(3, 10, 4, 10, 1) << endl;
cout << POLYNOMIAL(1, -3, -4) % POLYNOMIAL(3, 10, 4, 10, 1) << endl;
cout << POLYNOMIAL(1, 8, -4) / POLYNOMIAL(2, -6, 1, 2) << endl;
cout << POLYNOMIAL(1, 8, -4) % POLYNOMIAL(2, -6, 1, 2) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(1, 10, -8) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(1, 10, -8) << endl;
cout << POLYNOMIAL(3, 4, -10, -2, -3) / POLYNOMIAL(3, 9, -9, -10, -3) << endl;
cout << POLYNOMIAL(3, 4, -10, -2, -3) % POLYNOMIAL(3, 9, -9, -10, -3) << endl;
cout << POLYNOMIAL(2, 3, -2, -4) / POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(2, 3, -2, -4) % POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(1, 6, -6) / POLYNOMIAL(2, -5, 6, 5) << endl;
cout << POLYNOMIAL(1, 6, -6) % POLYNOMIAL(2, -5, 6, 5) << endl;
cout << POLYNOMIAL(3, -3, -6, -9, 1) / POLYNOMIAL(1, 1, 8) << endl;
cout << POLYNOMIAL(3, -3, -6, -9, 1) % POLYNOMIAL(1, 1, 8) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(1, -6, -1) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(1, -6, -1) << endl;
cout << POLYNOMIAL(3, 9, 2, 7, 3) / POLYNOMIAL(3, 9, 5, 3, 0) << endl;
cout << POLYNOMIAL(3, 9, 2, 7, 3) % POLYNOMIAL(3, 9, 5, 3, 0) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(3, 8, -10, -6, -9) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(3, 8, -10, -6, -9) << endl;
cout << POLYNOMIAL(3, -9, -5, -7, -5) / POLYNOMIAL(1, 5, 5) << endl;
cout << POLYNOMIAL(3, -9, -5, -7, -5) % POLYNOMIAL(1, 5, 5) << endl;
cout << POLYNOMIAL(1, -10, -3) / POLYNOMIAL(1, 0, 10) << endl;
cout << POLYNOMIAL(1, -10, -3) % POLYNOMIAL(1, 0, 10) << endl;
cout << POLYNOMIAL(1, -7, -1) / POLYNOMIAL(2, 7, -3, 8) << endl;
cout << POLYNOMIAL(1, -7, -1) % POLYNOMIAL(2, 7, -3, 8) << endl;
cout << POLYNOMIAL(1, -8, -4) / POLYNOMIAL(2, 10, -9, -5) << endl;
cout << POLYNOMIAL(1, -8, -4) % POLYNOMIAL(2, 10, -9, -5) << endl;
cout << POLYNOMIAL(2, -4, -10, 9) / POLYNOMIAL(2, 1, 8, -9) << endl;
cout << POLYNOMIAL(2, -4, -10, 9) % POLYNOMIAL(2, 1, 8, -9) << endl;
cout << POLYNOMIAL(1, 4, -7) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(1, 4, -7) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(2, -1, -9, 4) / POLYNOMIAL(2, 6, 1, 8) << endl;
cout << POLYNOMIAL(2, -1, -9, 4) % POLYNOMIAL(2, 6, 1, 8) << endl;
cout << POLYNOMIAL(3, -9, 8, 9, -10) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -9, 8, 9, -10) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -6, 5, -4, 7) / POLYNOMIAL(3, -7, 7, 2, -4) << endl;
cout << POLYNOMIAL(3, -6, 5, -4, 7) % POLYNOMIAL(3, -7, 7, 2, -4) << endl;
cout << POLYNOMIAL(2, 2, 8, -4) / POLYNOMIAL(3, 8, -7, -7, 4) << endl;
cout << POLYNOMIAL(2, 2, 8, -4) % POLYNOMIAL(3, 8, -7, -7, 4) << endl;
cout << POLYNOMIAL(3, 0, 0, 10, -10) / POLYNOMIAL(1, -10, -7) << endl;
cout << POLYNOMIAL(3, 0, 0, 10, -10) % POLYNOMIAL(1, -10, -7) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(3, -10, -8, 4, -4) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(3, -10, -8, 4, -4) << endl;
cout << POLYNOMIAL(1, -2, 6) / POLYNOMIAL(3, 0, -9, -2, 9) << endl;
cout << POLYNOMIAL(1, -2, 6) % POLYNOMIAL(3, 0, -9, -2, 9) << endl;
cout << POLYNOMIAL(3, 1, -6, 7, 6) / POLYNOMIAL(3, -7, 7, -9, -10) << endl;
cout << POLYNOMIAL(3, 1, -6, 7, 6) % POLYNOMIAL(3, -7, 7, -9, -10) << endl;
cout << POLYNOMIAL(1, -3, -5) / POLYNOMIAL(2, -5, 10, 1) << endl;
cout << POLYNOMIAL(1, -3, -5) % POLYNOMIAL(2, -5, 10, 1) << endl;
cout << POLYNOMIAL(2, 0, -1, -6) / POLYNOMIAL(1, -2, 2) << endl;
cout << POLYNOMIAL(2, 0, -1, -6) % POLYNOMIAL(1, -2, 2) << endl;
cout << POLYNOMIAL(3, -1, 8, 10, -6) / POLYNOMIAL(3, -5, -8, 0, 7) << endl;
cout << POLYNOMIAL(3, -1, 8, 10, -6) % POLYNOMIAL(3, -5, -8, 0, 7) << endl;
cout << POLYNOMIAL(1, 7, -3) / POLYNOMIAL(3, 7, -1, 2, 7) << endl;
cout << POLYNOMIAL(1, 7, -3) % POLYNOMIAL(3, 7, -1, 2, 7) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(2, -5, 5, -6) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(2, -5, 5, -6) << endl;
cout << POLYNOMIAL(1, 2, 2) / POLYNOMIAL(1, -4, -10) << endl;
cout << POLYNOMIAL(1, 2, 2) % POLYNOMIAL(1, -4, -10) << endl;
cout << POLYNOMIAL(1, -9, -2) / POLYNOMIAL(2, -3, -1, 6) << endl;
cout << POLYNOMIAL(1, -9, -2) % POLYNOMIAL(2, -3, -1, 6) << endl;
cout << POLYNOMIAL(2, 5, 10, -3) / POLYNOMIAL(3, -1, 8, -9, 1) << endl;
cout << POLYNOMIAL(2, 5, 10, -3) % POLYNOMIAL(3, -1, 8, -9, 1) << endl;
cout << POLYNOMIAL(0, 2) / POLYNOMIAL(2, -8, 6, 5) << endl;
cout << POLYNOMIAL(0, 2) % POLYNOMIAL(2, -8, 6, 5) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(3, 1, -8, -5, -4) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(3, 1, -8, -5, -4) << endl;
cout << POLYNOMIAL(2, 5, -5, -2) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, 5, -5, -2) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(0, 5) / POLYNOMIAL(3, 4, -9, 7, -1) << endl;
cout << POLYNOMIAL(0, 5) % POLYNOMIAL(3, 4, -9, 7, -1) << endl;
cout << POLYNOMIAL(1, 2, 9) / POLYNOMIAL(2, -6, 0, -10) << endl;
cout << POLYNOMIAL(1, 2, 9) % POLYNOMIAL(2, -6, 0, -10) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(3, -3, -9, 9, -6) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(3, -3, -9, 9, -6) << endl;
cout << POLYNOMIAL(3, -4, 1, 5, -7) / POLYNOMIAL(3, -7, 8, 4, -7) << endl;
cout << POLYNOMIAL(3, -4, 1, 5, -7) % POLYNOMIAL(3, -7, 8, 4, -7) << endl;
cout << POLYNOMIAL(2, 5, -9, 1) / POLYNOMIAL(1, -5, -1) << endl;
cout << POLYNOMIAL(2, 5, -9, 1) % POLYNOMIAL(1, -5, -1) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(1, -5, 7) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(1, -5, 7) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(2, -1, -1, 0) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(2, -1, -1, 0) << endl;
cout << POLYNOMIAL(3, -9, 10, -7, -8) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -9, 10, -7, -8) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(1, -8, -9) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(1, -8, -9) << endl;
cout << POLYNOMIAL(1, 9, 2) / POLYNOMIAL(1, 4, 6) << endl;
cout << POLYNOMIAL(1, 9, 2) % POLYNOMIAL(1, 4, 6) << endl;
cout << POLYNOMIAL(2, 5, -6, -9) / POLYNOMIAL(2, -1, -3, 3) << endl;
cout << POLYNOMIAL(2, 5, -6, -9) % POLYNOMIAL(2, -1, -3, 3) << endl;
cout << POLYNOMIAL(1, -5, 9) / POLYNOMIAL(1, -6, 3) << endl;
cout << POLYNOMIAL(1, -5, 9) % POLYNOMIAL(1, -6, 3) << endl;
cout << POLYNOMIAL(2, -10, 9, -7) / POLYNOMIAL(1, -2, 9) << endl;
cout << POLYNOMIAL(2, -10, 9, -7) % POLYNOMIAL(1, -2, 9) << endl;
cout << POLYNOMIAL(1, -5, -10) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, -5, -10) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, 3, -7, 4) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(2, 3, -7, 4) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(1, 4, 5) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(1, 4, 5) << endl;
cout << POLYNOMIAL(1, -9, 7) / POLYNOMIAL(3, -4, -7, -7, 10) << endl;
cout << POLYNOMIAL(1, -9, 7) % POLYNOMIAL(3, -4, -7, -7, 10) << endl;
cout << POLYNOMIAL(1, 7, 10) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(1, 7, 10) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(2, 7, 5, 8) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(2, 7, 5, 8) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(3, -5, -2, 6, 5) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(3, -5, -2, 6, 5) << endl;
cout << POLYNOMIAL(3, 7, 2, -9, 2) / POLYNOMIAL(2, 1, -10, 3) << endl;
cout << POLYNOMIAL(3, 7, 2, -9, 2) % POLYNOMIAL(2, 1, -10, 3) << endl;
cout << POLYNOMIAL(0, 6) / POLYNOMIAL(3, -9, -1, 4, 5) << endl;
cout << POLYNOMIAL(0, 6) % POLYNOMIAL(3, -9, -1, 4, 5) << endl;
cout << POLYNOMIAL(3, -9, 9, 6, -2) / POLYNOMIAL(3, -8, -1, 8, 6) << endl;
cout << POLYNOMIAL(3, -9, 9, 6, -2) % POLYNOMIAL(3, -8, -1, 8, 6) << endl;
cout << POLYNOMIAL(2, -10, 3, -3) / POLYNOMIAL(1, -9, -10) << endl;
cout << POLYNOMIAL(2, -10, 3, -3) % POLYNOMIAL(1, -9, -10) << endl;
cout << POLYNOMIAL(1, 9, -6) / POLYNOMIAL(1, 2, -2) << endl;
cout << POLYNOMIAL(1, 9, -6) % POLYNOMIAL(1, 2, -2) << endl;
cout << POLYNOMIAL(3, 10, 10, -7, -10) / POLYNOMIAL(2, 5, -1, 7) << endl;
cout << POLYNOMIAL(3, 10, 10, -7, -10) % POLYNOMIAL(2, 5, -1, 7) << endl;
cout << POLYNOMIAL(3, -4, -7, -1, 6) / POLYNOMIAL(1, 10, 2) << endl;
cout << POLYNOMIAL(3, -4, -7, -1, 6) % POLYNOMIAL(1, 10, 2) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(1, -6, -6) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(1, -6, -6) << endl;
cout << POLYNOMIAL(2, 4, -6, -5) / POLYNOMIAL(2, 10, -1, -4) << endl;
cout << POLYNOMIAL(2, 4, -6, -5) % POLYNOMIAL(2, 10, -1, -4) << endl;
cout << POLYNOMIAL(3, 6, 7, -7, -4) / POLYNOMIAL(2, 9, 7, -9) << endl;
cout << POLYNOMIAL(3, 6, 7, -7, -4) % POLYNOMIAL(2, 9, 7, -9) << endl;
cout << POLYNOMIAL(2, 7, -3, -1) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, 7, -3, -1) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(3, -10, -10, 4, 3) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -10, -10, 4, 3) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(2, 5, 10, 3) / POLYNOMIAL(3, -5, 1, -8, -1) << endl;
cout << POLYNOMIAL(2, 5, 10, 3) % POLYNOMIAL(3, -5, 1, -8, -1) << endl;
cout << POLYNOMIAL(1, 3, 4) / POLYNOMIAL(1, -7, 3) << endl;
cout << POLYNOMIAL(1, 3, 4) % POLYNOMIAL(1, -7, 3) << endl;
cout << POLYNOMIAL(2, 5, 10, -2) / POLYNOMIAL(2, 10, -10, 3) << endl;
cout << POLYNOMIAL(2, 5, 10, -2) % POLYNOMIAL(2, 10, -10, 3) << endl;
cout << POLYNOMIAL(2, -3, -6, -8) / POLYNOMIAL(1, -3, 2) << endl;
cout << POLYNOMIAL(2, -3, -6, -8) % POLYNOMIAL(1, -3, 2) << endl;
cout << POLYNOMIAL(3, -9, 5, -6, -9) / POLYNOMIAL(2, 8, -2, -2) << endl;
cout << POLYNOMIAL(3, -9, 5, -6, -9) % POLYNOMIAL(2, 8, -2, -2) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(3, -8, -2, -3, 1) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(3, -8, -2, -3, 1) << endl;
cout << POLYNOMIAL(3, -4, 3, -4, 6) / POLYNOMIAL(2, -1, 9, -4) << endl;
cout << POLYNOMIAL(3, -4, 3, -4, 6) % POLYNOMIAL(2, -1, 9, -4) << endl;
cout << POLYNOMIAL(1, 2, 9) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(1, 2, 9) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(3, -6, 4, 1, -8) / POLYNOMIAL(3, -5, 0, -9, 10) << endl;
cout << POLYNOMIAL(3, -6, 4, 1, -8) % POLYNOMIAL(3, -5, 0, -9, 10) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(2, -9, -2, 4) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(2, -9, -2, 4) << endl;
cout << POLYNOMIAL(3, -9, 2, -4, 9) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(3, -9, 2, -4, 9) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(1, -7, -9) / POLYNOMIAL(3, 5, 1, -3, -6) << endl;
cout << POLYNOMIAL(1, -7, -9) % POLYNOMIAL(3, 5, 1, -3, -6) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(1, -1, -3) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(1, -1, -3) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(3, 10, 8, -2, 5) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(3, 10, 8, -2, 5) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(2, 0, 0, -1) / POLYNOMIAL(1, -2, 9) << endl;
cout << POLYNOMIAL(2, 0, 0, -1) % POLYNOMIAL(1, -2, 9) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(1, 6, -9) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(1, 6, -9) << endl;
cout << POLYNOMIAL(3, 7, 9, -2, -2) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(3, 7, 9, -2, -2) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(1, 7, 2) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(1, 7, 2) << endl;
cout << POLYNOMIAL(2, -3, 4, 10) / POLYNOMIAL(1, -2, 7) << endl;
cout << POLYNOMIAL(2, -3, 4, 10) % POLYNOMIAL(1, -2, 7) << endl;
cout << POLYNOMIAL(2, -8, 8, -7) / POLYNOMIAL(1, 2, 8) << endl;
cout << POLYNOMIAL(2, -8, 8, -7) % POLYNOMIAL(1, 2, 8) << endl;
cout << POLYNOMIAL(3, -8, 6, 2, 0) / POLYNOMIAL(2, -2, 0, 2) << endl;
cout << POLYNOMIAL(3, -8, 6, 2, 0) % POLYNOMIAL(2, -2, 0, 2) << endl;
cout << POLYNOMIAL(3, -5, -1, -7, 7) / POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(3, -5, -1, -7, 7) % POLYNOMIAL(0, 8) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(1, 6, -2) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(1, 6, -2) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(2, 3, 1, 5) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(2, 3, 1, 5) << endl;
cout << POLYNOMIAL(2, 10, -10, 8) / POLYNOMIAL(1, -7, -8) << endl;
cout << POLYNOMIAL(2, 10, -10, 8) % POLYNOMIAL(1, -7, -8) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(3, 2, -2, 0, 3) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(3, 2, -2, 0, 3) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(3, 2, 9, 3, 10) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(3, 2, 9, 3, 10) << endl;
cout << POLYNOMIAL(1, -3, 5) / POLYNOMIAL(1, 4, 4) << endl;
cout << POLYNOMIAL(1, -3, 5) % POLYNOMIAL(1, 4, 4) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(3, 2, 4, 8, -8) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(3, 2, 4, 8, -8) << endl;
cout << POLYNOMIAL(1, -5, -3) / POLYNOMIAL(3, 1, -3, -9, -5) << endl;
cout << POLYNOMIAL(1, -5, -3) % POLYNOMIAL(3, 1, -3, -9, -5) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(3, -2, -4, -5, -3) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(3, -2, -4, -5, -3) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(2, -1, 9, 0) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(2, -1, 9, 0) << endl;
cout << POLYNOMIAL(2, 5, 6, 7) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(2, 5, 6, 7) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(2, 5, 0, 1) / POLYNOMIAL(1, -3, 2) << endl;
cout << POLYNOMIAL(2, 5, 0, 1) % POLYNOMIAL(1, -3, 2) << endl;
cout << POLYNOMIAL(2, 4, 1, -5) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, 4, 1, -5) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -7, 3, -4, 7) / POLYNOMIAL(2, 7, 0, -4) << endl;
cout << POLYNOMIAL(3, -7, 3, -4, 7) % POLYNOMIAL(2, 7, 0, -4) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(2, 8, 3, -10) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(2, 8, 3, -10) << endl;
cout << POLYNOMIAL(2, -2, -10, -1) / POLYNOMIAL(1, -1, -1) << endl;
cout << POLYNOMIAL(2, -2, -10, -1) % POLYNOMIAL(1, -1, -1) << endl;
cout << POLYNOMIAL(3, 6, 10, 0, 2) / POLYNOMIAL(1, 3, -4) << endl;
cout << POLYNOMIAL(3, 6, 10, 0, 2) % POLYNOMIAL(1, 3, -4) << endl;
cout << POLYNOMIAL(3, 10, 2, -4, -1) / POLYNOMIAL(1, 0, -8) << endl;
cout << POLYNOMIAL(3, 10, 2, -4, -1) % POLYNOMIAL(1, 0, -8) << endl;
cout << POLYNOMIAL(1, 6, -6) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 6, -6) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 5, -1) / POLYNOMIAL(2, -7, 7, -10) << endl;
cout << POLYNOMIAL(1, 5, -1) % POLYNOMIAL(2, -7, 7, -10) << endl;
cout << POLYNOMIAL(2, 9, 2, -7) / POLYNOMIAL(3, -10, 4, -5, -4) << endl;
cout << POLYNOMIAL(2, 9, 2, -7) % POLYNOMIAL(3, -10, 4, -5, -4) << endl;
cout << POLYNOMIAL(1, 6, -4) / POLYNOMIAL(1, 0, 9) << endl;
cout << POLYNOMIAL(1, 6, -4) % POLYNOMIAL(1, 0, 9) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(3, -3, 4, 3, -10) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(3, -3, 4, 3, -10) << endl;
cout << POLYNOMIAL(1, 3, -4) / POLYNOMIAL(3, -6, 8, 1, -5) << endl;
cout << POLYNOMIAL(1, 3, -4) % POLYNOMIAL(3, -6, 8, 1, -5) << endl;
cout << POLYNOMIAL(3, -4, -10, -10, -6) / POLYNOMIAL(2, 2, 10, 1) << endl;
cout << POLYNOMIAL(3, -4, -10, -10, -6) % POLYNOMIAL(2, 2, 10, 1) << endl;
cout << POLYNOMIAL(2, -8, -6, -7) / POLYNOMIAL(3, -1, 10, 6, -8) << endl;
cout << POLYNOMIAL(2, -8, -6, -7) % POLYNOMIAL(3, -1, 10, 6, -8) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(1, 7, -10) / POLYNOMIAL(2, -1, 1, 6) << endl;
cout << POLYNOMIAL(1, 7, -10) % POLYNOMIAL(2, -1, 1, 6) << endl;
cout << POLYNOMIAL(2, 1, 9, -1) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, 1, 9, -1) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, 1, -3, 5) / POLYNOMIAL(1, 2, -6) << endl;
cout << POLYNOMIAL(2, 1, -3, 5) % POLYNOMIAL(1, 2, -6) << endl;
cout << POLYNOMIAL(3, -2, 3, -8, -6) / POLYNOMIAL(1, -4, -6) << endl;
cout << POLYNOMIAL(3, -2, 3, -8, -6) % POLYNOMIAL(1, -4, -6) << endl;
cout << POLYNOMIAL(1, -7, 10) / POLYNOMIAL(1, 5, 10) << endl;
cout << POLYNOMIAL(1, -7, 10) % POLYNOMIAL(1, 5, 10) << endl;
cout << POLYNOMIAL(3, 3, 10, -2, 8) / POLYNOMIAL(1, -1, -1) << endl;
cout << POLYNOMIAL(3, 3, 10, -2, 8) % POLYNOMIAL(1, -1, -1) << endl;
cout << POLYNOMIAL(1, 1, 0) / POLYNOMIAL(2, -2, -9, -9) << endl;
cout << POLYNOMIAL(1, 1, 0) % POLYNOMIAL(2, -2, -9, -9) << endl;
cout << POLYNOMIAL(1, -8, -5) / POLYNOMIAL(3, -9, 8, 7, 0) << endl;
cout << POLYNOMIAL(1, -8, -5) % POLYNOMIAL(3, -9, 8, 7, 0) << endl;
cout << POLYNOMIAL(1, -1, 9) / POLYNOMIAL(1, -7, 9) << endl;
cout << POLYNOMIAL(1, -1, 9) % POLYNOMIAL(1, -7, 9) << endl;
cout << POLYNOMIAL(1, 2, 3) / POLYNOMIAL(3, -4, -7, 9, 5) << endl;
cout << POLYNOMIAL(1, 2, 3) % POLYNOMIAL(3, -4, -7, 9, 5) << endl;
cout << POLYNOMIAL(2, 6, -3, 4) / POLYNOMIAL(2, 7, -8, -9) << endl;
cout << POLYNOMIAL(2, 6, -3, 4) % POLYNOMIAL(2, 7, -8, -9) << endl;
cout << POLYNOMIAL(2, 9, 9, -7) / POLYNOMIAL(1, 8, 7) << endl;
cout << POLYNOMIAL(2, 9, 9, -7) % POLYNOMIAL(1, 8, 7) << endl;
cout << POLYNOMIAL(1, 4, 7) / POLYNOMIAL(1, 9, 1) << endl;
cout << POLYNOMIAL(1, 4, 7) % POLYNOMIAL(1, 9, 1) << endl;
cout << POLYNOMIAL(1, 4, -10) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(1, 4, -10) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(1, 10, -2) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(1, 10, -2) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(2, 6, 9, -5) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(2, 6, 9, -5) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, -2) / POLYNOMIAL(1, -5, -5) << endl;
cout << POLYNOMIAL(0, -2) % POLYNOMIAL(1, -5, -5) << endl;
cout << POLYNOMIAL(1, -7, -10) / POLYNOMIAL(2, 10, 10, -9) << endl;
cout << POLYNOMIAL(1, -7, -10) % POLYNOMIAL(2, 10, 10, -9) << endl;
cout << POLYNOMIAL(2, -6, 8, 5) / POLYNOMIAL(1, 7, -5) << endl;
cout << POLYNOMIAL(2, -6, 8, 5) % POLYNOMIAL(1, 7, -5) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(2, 2, 0, 0) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(2, 2, 0, 0) << endl;
cout << POLYNOMIAL(1, 0, -1) / POLYNOMIAL(1, -2, 2) << endl;
cout << POLYNOMIAL(1, 0, -1) % POLYNOMIAL(1, -2, 2) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(1, 4, 10) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(1, 4, 10) << endl;
cout << POLYNOMIAL(3, -6, -5, 6, -3) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, -6, -5, 6, -3) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, -7, -4, -3, -5) / POLYNOMIAL(3, 7, 8, 10, 9) << endl;
cout << POLYNOMIAL(3, -7, -4, -3, -5) % POLYNOMIAL(3, 7, 8, 10, 9) << endl;
cout << POLYNOMIAL(3, -3, 2, -6, 3) / POLYNOMIAL(3, -5, 0, -6, -2) << endl;
cout << POLYNOMIAL(3, -3, 2, -6, 3) % POLYNOMIAL(3, -5, 0, -6, -2) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(1, 5, 4) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(1, 5, 4) << endl;
cout << POLYNOMIAL(3, 1, 2, 1, 10) / POLYNOMIAL(1, 1, -6) << endl;
cout << POLYNOMIAL(3, 1, 2, 1, 10) % POLYNOMIAL(1, 1, -6) << endl;
cout << POLYNOMIAL(3, -10, 10, 7, 1) / POLYNOMIAL(1, 8, -10) << endl;
cout << POLYNOMIAL(3, -10, 10, 7, 1) % POLYNOMIAL(1, 8, -10) << endl;
cout << POLYNOMIAL(2, -7, 6, -4) / POLYNOMIAL(3, 10, 10, -3, 7) << endl;
cout << POLYNOMIAL(2, -7, 6, -4) % POLYNOMIAL(3, 10, 10, -3, 7) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(3, 3, 7, 2, 0) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(3, 3, 7, 2, 0) << endl;
cout << POLYNOMIAL(3, 7, 5, -9, 0) / POLYNOMIAL(2, -5, 0, 6) << endl;
cout << POLYNOMIAL(3, 7, 5, -9, 0) % POLYNOMIAL(2, -5, 0, 6) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(2, -8, 9, -3) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(2, -8, 9, -3) << endl;
cout << POLYNOMIAL(3, -8, -5, 0, 5) / POLYNOMIAL(3, 10, -2, 6, 2) << endl;
cout << POLYNOMIAL(3, -8, -5, 0, 5) % POLYNOMIAL(3, 10, -2, 6, 2) << endl;
cout << POLYNOMIAL(0, -5) / POLYNOMIAL(2, 1, 8, 3) << endl;
cout << POLYNOMIAL(0, -5) % POLYNOMIAL(2, 1, 8, 3) << endl;
cout << POLYNOMIAL(2, 1, 5, -2) / POLYNOMIAL(2, -1, 4, 10) << endl;
cout << POLYNOMIAL(2, 1, 5, -2) % POLYNOMIAL(2, -1, 4, 10) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(2, 8, -5, 5) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(2, 8, -5, 5) << endl;
cout << POLYNOMIAL(1, 1, -6) / POLYNOMIAL(2, -4, 0, -1) << endl;
cout << POLYNOMIAL(1, 1, -6) % POLYNOMIAL(2, -4, 0, -1) << endl;
cout << POLYNOMIAL(0, 6) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(0, 6) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, -1, 8, -7) / POLYNOMIAL(3, 2, 8, 0, -4) << endl;
cout << POLYNOMIAL(2, -1, 8, -7) % POLYNOMIAL(3, 2, 8, 0, -4) << endl;
cout << POLYNOMIAL(3, -1, 4, 9, 1) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -1, 4, 9, 1) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, 4, -10, 1, -9) / POLYNOMIAL(1, 0, -3) << endl;
cout << POLYNOMIAL(3, 4, -10, 1, -9) % POLYNOMIAL(1, 0, -3) << endl;
cout << POLYNOMIAL(3, 3, 10, 0, 6) / POLYNOMIAL(1, -8, -2) << endl;
cout << POLYNOMIAL(3, 3, 10, 0, 6) % POLYNOMIAL(1, -8, -2) << endl;
cout << POLYNOMIAL(3, -4, 10, -1, -3) / POLYNOMIAL(3, 4, 1, -9, 2) << endl;
cout << POLYNOMIAL(3, -4, 10, -1, -3) % POLYNOMIAL(3, 4, 1, -9, 2) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(1, 8, 5) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(1, 8, 5) << endl;
cout << POLYNOMIAL(2, -9, 9, -6) / POLYNOMIAL(3, -9, 8, 1, -3) << endl;
cout << POLYNOMIAL(2, -9, 9, -6) % POLYNOMIAL(3, -9, 8, 1, -3) << endl;
cout << POLYNOMIAL(3, 8, 5, 1, 0) / POLYNOMIAL(3, -10, 3, 5, 0) << endl;
cout << POLYNOMIAL(3, 8, 5, 1, 0) % POLYNOMIAL(3, -10, 3, 5, 0) << endl;
cout << POLYNOMIAL(3, 1, 9, 5, -1) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(3, 1, 9, 5, -1) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(1, 1, -10) / POLYNOMIAL(1, -6, -7) << endl;
cout << POLYNOMIAL(1, 1, -10) % POLYNOMIAL(1, -6, -7) << endl;
cout << POLYNOMIAL(1, 3, -6) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(1, 3, -6) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(1, -9, 5) / POLYNOMIAL(1, -1, 6) << endl;
cout << POLYNOMIAL(1, -9, 5) % POLYNOMIAL(1, -1, 6) << endl;
cout << POLYNOMIAL(3, 1, -9, -8, -4) / POLYNOMIAL(2, 7, 3, 1) << endl;
cout << POLYNOMIAL(3, 1, -9, -8, -4) % POLYNOMIAL(2, 7, 3, 1) << endl;
cout << POLYNOMIAL(2, 2, 6, -8) / POLYNOMIAL(1, -1, -4) << endl;
cout << POLYNOMIAL(2, 2, 6, -8) % POLYNOMIAL(1, -1, -4) << endl;
cout << POLYNOMIAL(2, 4, -4, -6) / POLYNOMIAL(2, 6, 7, 8) << endl;
cout << POLYNOMIAL(2, 4, -4, -6) % POLYNOMIAL(2, 6, 7, 8) << endl;
cout << POLYNOMIAL(3, 4, 6, 3, 5) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(3, 4, 6, 3, 5) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(3, 5, 7, 9, 5) / POLYNOMIAL(2, -3, -6, -8) << endl;
cout << POLYNOMIAL(3, 5, 7, 9, 5) % POLYNOMIAL(2, -3, -6, -8) << endl;
cout << POLYNOMIAL(1, -8, -5) / POLYNOMIAL(3, -3, 3, -1, 9) << endl;
cout << POLYNOMIAL(1, -8, -5) % POLYNOMIAL(3, -3, 3, -1, 9) << endl;
cout << POLYNOMIAL(1, 9, 5) / POLYNOMIAL(2, 4, 1, 5) << endl;
cout << POLYNOMIAL(1, 9, 5) % POLYNOMIAL(2, 4, 1, 5) << endl;
cout << POLYNOMIAL(2, 2, -7, -8) / POLYNOMIAL(2, 7, 3, -7) << endl;
cout << POLYNOMIAL(2, 2, -7, -8) % POLYNOMIAL(2, 7, 3, -7) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(3, -8, -10, 8, -2) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(3, -8, -10, 8, -2) << endl;
cout << POLYNOMIAL(1, 5, 4) / POLYNOMIAL(1, 1, -9) << endl;
cout << POLYNOMIAL(1, 5, 4) % POLYNOMIAL(1, 1, -9) << endl;
cout << POLYNOMIAL(3, 7, 5, 10, 6) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(3, 7, 5, 10, 6) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(1, 5, 3) / POLYNOMIAL(3, 7, 3, -5, 10) << endl;
cout << POLYNOMIAL(1, 5, 3) % POLYNOMIAL(3, 7, 3, -5, 10) << endl;
cout << POLYNOMIAL(2, -9, -8, 4) / POLYNOMIAL(2, -8, 1, -4) << endl;
cout << POLYNOMIAL(2, -9, -8, 4) % POLYNOMIAL(2, -8, 1, -4) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(1, -2, 9) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(1, -2, 9) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, 8, 2, 7) / POLYNOMIAL(3, -8, 5, 4, -9) << endl;
cout << POLYNOMIAL(2, 8, 2, 7) % POLYNOMIAL(3, -8, 5, 4, -9) << endl;
cout << POLYNOMIAL(3, 1, 9, 9, -7) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(3, 1, 9, 9, -7) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(0, 6) / POLYNOMIAL(3, -2, -2, 6, 1) << endl;
cout << POLYNOMIAL(0, 6) % POLYNOMIAL(3, -2, -2, 6, 1) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(1, 1, -7) / POLYNOMIAL(2, 10, -6, 3) << endl;
cout << POLYNOMIAL(1, 1, -7) % POLYNOMIAL(2, 10, -6, 3) << endl;
cout << POLYNOMIAL(2, -1, -10, -4) / POLYNOMIAL(2, 9, -3, 1) << endl;
cout << POLYNOMIAL(2, -1, -10, -4) % POLYNOMIAL(2, 9, -3, 1) << endl;
cout << POLYNOMIAL(3, 9, -6, 10, 1) / POLYNOMIAL(2, -8, -5, 10) << endl;
cout << POLYNOMIAL(3, 9, -6, 10, 1) % POLYNOMIAL(2, -8, -5, 10) << endl;
cout << POLYNOMIAL(2, 7, -9, 1) / POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(2, 7, -9, 1) % POLYNOMIAL(0, -9) << endl;
cout << POLYNOMIAL(0, 5) / POLYNOMIAL(2, 9, -5, -1) << endl;
cout << POLYNOMIAL(0, 5) % POLYNOMIAL(2, 9, -5, -1) << endl;
cout << POLYNOMIAL(1, 5, -8) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, 5, -8) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(1, 3, 0) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(1, 3, 0) << endl;
cout << POLYNOMIAL(3, 8, 6, -8, -2) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, 8, 6, -8, -2) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(1, -3, 5) / POLYNOMIAL(2, 4, -8, 1) << endl;
cout << POLYNOMIAL(1, -3, 5) % POLYNOMIAL(2, 4, -8, 1) << endl;
cout << POLYNOMIAL(1, 1, -7) / POLYNOMIAL(3, 5, 1, 7, -8) << endl;
cout << POLYNOMIAL(1, 1, -7) % POLYNOMIAL(3, 5, 1, 7, -8) << endl;
cout << POLYNOMIAL(1, -7, -4) / POLYNOMIAL(1, -8, -4) << endl;
cout << POLYNOMIAL(1, -7, -4) % POLYNOMIAL(1, -8, -4) << endl;
cout << POLYNOMIAL(3, -8, 10, 8, -4) / POLYNOMIAL(1, -6, 1) << endl;
cout << POLYNOMIAL(3, -8, 10, 8, -4) % POLYNOMIAL(1, -6, 1) << endl;
cout << POLYNOMIAL(1, -1, -3) / POLYNOMIAL(1, -10, 6) << endl;
cout << POLYNOMIAL(1, -1, -3) % POLYNOMIAL(1, -10, 6) << endl;
cout << POLYNOMIAL(3, 8, 0, -7, 4) / POLYNOMIAL(2, 0, -3, -6) << endl;
cout << POLYNOMIAL(3, 8, 0, -7, 4) % POLYNOMIAL(2, 0, -3, -6) << endl;
cout << POLYNOMIAL(3, 1, -7, 1, 3) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, 1, -7, 1, 3) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -2, 2, -6, 8) / POLYNOMIAL(3, -3, -1, 1, -5) << endl;
cout << POLYNOMIAL(3, -2, 2, -6, 8) % POLYNOMIAL(3, -3, -1, 1, -5) << endl;
cout << POLYNOMIAL(2, 9, 7, 6) / POLYNOMIAL(3, -8, 2, 5, 7) << endl;
cout << POLYNOMIAL(2, 9, 7, 6) % POLYNOMIAL(3, -8, 2, 5, 7) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(1, -3, -7) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(1, -3, -7) << endl;
cout << POLYNOMIAL(3, 3, 9, 4, 3) / POLYNOMIAL(3, 3, 5, -5, -2) << endl;
cout << POLYNOMIAL(3, 3, 9, 4, 3) % POLYNOMIAL(3, 3, 5, -5, -2) << endl;
cout << POLYNOMIAL(3, -3, -5, -5, 6) / POLYNOMIAL(3, -4, 0, -6, 10) << endl;
cout << POLYNOMIAL(3, -3, -5, -5, 6) % POLYNOMIAL(3, -4, 0, -6, 10) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(3, -7, 5, -8, 0) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(3, -7, 5, -8, 0) << endl;
cout << POLYNOMIAL(2, 10, -4, -1) / POLYNOMIAL(2, -1, 5, 10) << endl;
cout << POLYNOMIAL(2, 10, -4, -1) % POLYNOMIAL(2, -1, 5, 10) << endl;
cout << POLYNOMIAL(1, 10, -1) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 10, -1) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(3, -9, 1, 1, -2) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(3, -9, 1, 1, -2) << endl;
cout << POLYNOMIAL(2, -10, 7, 9) / POLYNOMIAL(1, -4, 6) << endl;
cout << POLYNOMIAL(2, -10, 7, 9) % POLYNOMIAL(1, -4, 6) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(2, 2, -3, -1) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(2, 2, -3, -1) << endl;
cout << POLYNOMIAL(2, -6, 2, -1) / POLYNOMIAL(3, -3, -9, -5, -9) << endl;
cout << POLYNOMIAL(2, -6, 2, -1) % POLYNOMIAL(3, -3, -9, -5, -9) << endl;
cout << POLYNOMIAL(2, -10, 6, 9) / POLYNOMIAL(2, -7, -1, 3) << endl;
cout << POLYNOMIAL(2, -10, 6, 9) % POLYNOMIAL(2, -7, -1, 3) << endl;
cout << POLYNOMIAL(2, 6, -7, -9) / POLYNOMIAL(1, 5, 7) << endl;
cout << POLYNOMIAL(2, 6, -7, -9) % POLYNOMIAL(1, 5, 7) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(3, -8, 9, 1, 9) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(3, -8, 9, 1, 9) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(2, 4, -9, -3) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(2, 4, -9, -3) << endl;
cout << POLYNOMIAL(3, -9, -9, -8, -7) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, -9, -9, -8, -7) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(1, 3, 3) / POLYNOMIAL(3, -4, -1, 0, -1) << endl;
cout << POLYNOMIAL(1, 3, 3) % POLYNOMIAL(3, -4, -1, 0, -1) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(3, -4, -9, -4, 3) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(3, -4, -9, -4, 3) << endl;
cout << POLYNOMIAL(2, -1, 5, -4) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, -1, 5, -4) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, 9, 8) / POLYNOMIAL(1, 9, 6) << endl;
cout << POLYNOMIAL(1, 9, 8) % POLYNOMIAL(1, 9, 6) << endl;
cout << POLYNOMIAL(3, -4, 4, -5, 1) / POLYNOMIAL(2, -1, 8, 6) << endl;
cout << POLYNOMIAL(3, -4, 4, -5, 1) % POLYNOMIAL(2, -1, 8, 6) << endl;
cout << POLYNOMIAL(3, -7, 6, -10, 5) / POLYNOMIAL(3, -6, -10, 3, -7) << endl;
cout << POLYNOMIAL(3, -7, 6, -10, 5) % POLYNOMIAL(3, -6, -10, 3, -7) << endl;
cout << POLYNOMIAL(3, 2, -5, -6, 9) / POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(3, 2, -5, -6, 9) % POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(1, 6, 10) / POLYNOMIAL(1, -9, -2) << endl;
cout << POLYNOMIAL(1, 6, 10) % POLYNOMIAL(1, -9, -2) << endl;
cout << POLYNOMIAL(2, 9, 10, -9) / POLYNOMIAL(2, -6, 4, 4) << endl;
cout << POLYNOMIAL(2, 9, 10, -9) % POLYNOMIAL(2, -6, 4, 4) << endl;
cout << POLYNOMIAL(1, -6, 1) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(1, -6, 1) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(1, 0, 7) / POLYNOMIAL(2, 6, 10, 7) << endl;
cout << POLYNOMIAL(1, 0, 7) % POLYNOMIAL(2, 6, 10, 7) << endl;
cout << POLYNOMIAL(3, -6, 10, 2, -2) / POLYNOMIAL(3, -6, -3, -8, -3) << endl;
cout << POLYNOMIAL(3, -6, 10, 2, -2) % POLYNOMIAL(3, -6, -3, -8, -3) << endl;
cout << POLYNOMIAL(3, -10, 8, -6, 2) / POLYNOMIAL(3, -10, 6, 10, 4) << endl;
cout << POLYNOMIAL(3, -10, 8, -6, 2) % POLYNOMIAL(3, -10, 6, 10, 4) << endl;
cout << POLYNOMIAL(3, 3, 5, 9, 10) / POLYNOMIAL(3, -1, 0, -10, 3) << endl;
cout << POLYNOMIAL(3, 3, 5, 9, 10) % POLYNOMIAL(3, -1, 0, -10, 3) << endl;
cout << POLYNOMIAL(2, -3, 9, -1) / POLYNOMIAL(2, 9, 3, 5) << endl;
cout << POLYNOMIAL(2, -3, 9, -1) % POLYNOMIAL(2, 9, 3, 5) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(3, -3, -1, 2, -9) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(3, -3, -1, 2, -9) << endl;
cout << POLYNOMIAL(2, -6, -5, -2) / POLYNOMIAL(1, -1, 6) << endl;
cout << POLYNOMIAL(2, -6, -5, -2) % POLYNOMIAL(1, -1, 6) << endl;
cout << POLYNOMIAL(3, -2, 7, -3, 7) / POLYNOMIAL(3, 2, 8, -4, -7) << endl;
cout << POLYNOMIAL(3, -2, 7, -3, 7) % POLYNOMIAL(3, 2, 8, -4, -7) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(0, 4) / POLYNOMIAL(1, 2, -1) << endl;
cout << POLYNOMIAL(0, 4) % POLYNOMIAL(1, 2, -1) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, 7, 1, -4, 8) / POLYNOMIAL(2, -4, -3, -6) << endl;
cout << POLYNOMIAL(3, 7, 1, -4, 8) % POLYNOMIAL(2, -4, -3, -6) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(2, -4, 4, -2) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(2, -4, 4, -2) << endl;
cout << POLYNOMIAL(1, 5, 1) / POLYNOMIAL(2, -1, -1, 5) << endl;
cout << POLYNOMIAL(1, 5, 1) % POLYNOMIAL(2, -1, -1, 5) << endl;
cout << POLYNOMIAL(2, -5, 7, -6) / POLYNOMIAL(3, -7, 10, -5, 0) << endl;
cout << POLYNOMIAL(2, -5, 7, -6) % POLYNOMIAL(3, -7, 10, -5, 0) << endl;
cout << POLYNOMIAL(3, -3, -6, -4, 3) / POLYNOMIAL(3, -3, -3, 9, -6) << endl;
cout << POLYNOMIAL(3, -3, -6, -4, 3) % POLYNOMIAL(3, -3, -3, 9, -6) << endl;
cout << POLYNOMIAL(2, -6, 6, 2) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(2, -6, 6, 2) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(3, -8, -5, 6, -7) / POLYNOMIAL(3, 1, 7, -8, 6) << endl;
cout << POLYNOMIAL(3, -8, -5, 6, -7) % POLYNOMIAL(3, 1, 7, -8, 6) << endl;
cout << POLYNOMIAL(3, 5, -5, 9, -2) / POLYNOMIAL(1, -8, -6) << endl;
cout << POLYNOMIAL(3, 5, -5, 9, -2) % POLYNOMIAL(1, -8, -6) << endl;
cout << POLYNOMIAL(2, 2, 4, 9) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(2, 2, 4, 9) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -8, -8, -4, -8) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -8, -8, -4, -8) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -1, -3, -2, 10) / POLYNOMIAL(3, 5, -4, -5, 6) << endl;
cout << POLYNOMIAL(3, -1, -3, -2, 10) % POLYNOMIAL(3, 5, -4, -5, 6) << endl;
cout << POLYNOMIAL(1, 4, 3) / POLYNOMIAL(1, 10, 0) << endl;
cout << POLYNOMIAL(1, 4, 3) % POLYNOMIAL(1, 10, 0) << endl;
cout << POLYNOMIAL(3, 8, -1, 3, -6) / POLYNOMIAL(2, 2, 6, 2) << endl;
cout << POLYNOMIAL(3, 8, -1, 3, -6) % POLYNOMIAL(2, 2, 6, 2) << endl;
cout << POLYNOMIAL(1, 3, 2) / POLYNOMIAL(1, -3, 2) << endl;
cout << POLYNOMIAL(1, 3, 2) % POLYNOMIAL(1, -3, 2) << endl;
cout << POLYNOMIAL(3, 1, 3, -10, 10) / POLYNOMIAL(1, 9, 10) << endl;
cout << POLYNOMIAL(3, 1, 3, -10, 10) % POLYNOMIAL(1, 9, 10) << endl;
cout << POLYNOMIAL(2, -8, -7, 4) / POLYNOMIAL(2, -4, -8, 10) << endl;
cout << POLYNOMIAL(2, -8, -7, 4) % POLYNOMIAL(2, -4, -8, 10) << endl;
cout << POLYNOMIAL(1, -4, 5) / POLYNOMIAL(3, -9, -6, 3, -4) << endl;
cout << POLYNOMIAL(1, -4, 5) % POLYNOMIAL(3, -9, -6, 3, -4) << endl;
cout << POLYNOMIAL(1, -3, -8) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(1, -3, -8) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(3, 10, -9, -10, 2) / POLYNOMIAL(3, -2, -1, 9, -4) << endl;
cout << POLYNOMIAL(3, 10, -9, -10, 2) % POLYNOMIAL(3, -2, -1, 9, -4) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(3, 6, -6, -9, 9) / POLYNOMIAL(2, -4, 1, 6) << endl;
cout << POLYNOMIAL(3, 6, -6, -9, 9) % POLYNOMIAL(2, -4, 1, 6) << endl;
cout << POLYNOMIAL(2, 10, -3, 4) / POLYNOMIAL(3, 5, -4, 10, 10) << endl;
cout << POLYNOMIAL(2, 10, -3, 4) % POLYNOMIAL(3, 5, -4, 10, 10) << endl;
cout << POLYNOMIAL(2, 10, 4, 3) / POLYNOMIAL(1, 8, 8) << endl;
cout << POLYNOMIAL(2, 10, 4, 3) % POLYNOMIAL(1, 8, 8) << endl;
cout << POLYNOMIAL(3, 4, -7, -9, -5) / POLYNOMIAL(3, 0, -9, 0, -2) << endl;
cout << POLYNOMIAL(3, 4, -7, -9, -5) % POLYNOMIAL(3, 0, -9, 0, -2) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(3, -10, -10, -5, 6) / POLYNOMIAL(1, 3, -4) << endl;
cout << POLYNOMIAL(3, -10, -10, -5, 6) % POLYNOMIAL(1, 3, -4) << endl;
cout << POLYNOMIAL(1, 2, 0) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, 2, 0) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(3, 4, 3, 5, -3) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, 4, 3, 5, -3) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, -4, -4, -4) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(2, -4, -4, -4) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(1, 4, -1) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(1, 4, -1) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(2, 5, 8, 6) / POLYNOMIAL(2, -5, 9, -4) << endl;
cout << POLYNOMIAL(2, 5, 8, 6) % POLYNOMIAL(2, -5, 9, -4) << endl;
cout << POLYNOMIAL(3, 6, 3, 5, 5) / POLYNOMIAL(3, 7, -4, 7, -8) << endl;
cout << POLYNOMIAL(3, 6, 3, 5, 5) % POLYNOMIAL(3, 7, -4, 7, -8) << endl;
cout << POLYNOMIAL(1, 5, -6) / POLYNOMIAL(1, -5, -9) << endl;
cout << POLYNOMIAL(1, 5, -6) % POLYNOMIAL(1, -5, -9) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(3, -9, 10, -6, 10) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(3, -9, 10, -6, 10) << endl;
cout << POLYNOMIAL(1, 6, 2) / POLYNOMIAL(2, -6, -3, -4) << endl;
cout << POLYNOMIAL(1, 6, 2) % POLYNOMIAL(2, -6, -3, -4) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(3, -10, -10, 3, 2) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(3, -10, -10, 3, 2) << endl;
cout << POLYNOMIAL(3, 6, 10, -4, 1) / POLYNOMIAL(1, 10, 10) << endl;
cout << POLYNOMIAL(3, 6, 10, -4, 1) % POLYNOMIAL(1, 10, 10) << endl;
cout << POLYNOMIAL(3, -7, -10, 9, -1) / POLYNOMIAL(3, -7, 9, 4, 8) << endl;
cout << POLYNOMIAL(3, -7, -10, 9, -1) % POLYNOMIAL(3, -7, 9, 4, 8) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(3, 3, 9, 0, -5) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(3, 3, 9, 0, -5) << endl;
cout << POLYNOMIAL(3, 9, 1, 3, -9) / POLYNOMIAL(1, 10, 0) << endl;
cout << POLYNOMIAL(3, 9, 1, 3, -9) % POLYNOMIAL(1, 10, 0) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(3, 7, -9, -8, 7) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(3, 7, -9, -8, 7) << endl;
cout << POLYNOMIAL(1, -5, 9) / POLYNOMIAL(2, 4, -1, -9) << endl;
cout << POLYNOMIAL(1, -5, 9) % POLYNOMIAL(2, 4, -1, -9) << endl;
cout << POLYNOMIAL(2, 2, -6, 0) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(2, 2, -6, 0) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(1, -1, 5) / POLYNOMIAL(3, -7, 4, 10, -1) << endl;
cout << POLYNOMIAL(1, -1, 5) % POLYNOMIAL(3, -7, 4, 10, -1) << endl;
cout << POLYNOMIAL(1, 3, -8) / POLYNOMIAL(2, -10, -7, -9) << endl;
cout << POLYNOMIAL(1, 3, -8) % POLYNOMIAL(2, -10, -7, -9) << endl;
cout << POLYNOMIAL(3, 10, 3, 4, -4) / POLYNOMIAL(1, -8, 2) << endl;
cout << POLYNOMIAL(3, 10, 3, 4, -4) % POLYNOMIAL(1, -8, 2) << endl;
cout << POLYNOMIAL(2, 3, 3, -3) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, 3, 3, -3) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, 8, 6) / POLYNOMIAL(1, -6, 9) << endl;
cout << POLYNOMIAL(1, 8, 6) % POLYNOMIAL(1, -6, 9) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(2, -3, -1, -2) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(2, -3, -1, -2) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(2, -4, 2, -9) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(2, -4, 2, -9) << endl;
cout << POLYNOMIAL(1, 8, -4) / POLYNOMIAL(3, -1, -10, 9, 9) << endl;
cout << POLYNOMIAL(1, 8, -4) % POLYNOMIAL(3, -1, -10, 9, 9) << endl;
cout << POLYNOMIAL(2, 3, 6, -7) / POLYNOMIAL(3, 10, 3, -9, 8) << endl;
cout << POLYNOMIAL(2, 3, 6, -7) % POLYNOMIAL(3, 10, 3, -9, 8) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(2, -9, -4, -1) / POLYNOMIAL(2, -4, 3, 4) << endl;
cout << POLYNOMIAL(2, -9, -4, -1) % POLYNOMIAL(2, -4, 3, 4) << endl;
cout << POLYNOMIAL(2, 4, 1, -2) / POLYNOMIAL(1, -10, -8) << endl;
cout << POLYNOMIAL(2, 4, 1, -2) % POLYNOMIAL(1, -10, -8) << endl;
cout << POLYNOMIAL(2, -9, 9, -1) / POLYNOMIAL(2, -3, 10, 3) << endl;
cout << POLYNOMIAL(2, -9, 9, -1) % POLYNOMIAL(2, -3, 10, 3) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(2, -7, 1, 2) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(2, -7, 1, 2) << endl;
cout << POLYNOMIAL(3, -3, 9, -10, 2) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(3, -3, 9, -10, 2) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(1, -9, -3) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(1, -9, -3) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(1, 8, 7) / POLYNOMIAL(3, -10, 8, 4, -5) << endl;
cout << POLYNOMIAL(1, 8, 7) % POLYNOMIAL(3, -10, 8, 4, -5) << endl;
cout << POLYNOMIAL(3, 0, 2, 10, 1) / POLYNOMIAL(2, 1, 9, -6) << endl;
cout << POLYNOMIAL(3, 0, 2, 10, 1) % POLYNOMIAL(2, 1, 9, -6) << endl;
cout << POLYNOMIAL(3, -3, 1, 10, 6) / POLYNOMIAL(2, 10, -5, -2) << endl;
cout << POLYNOMIAL(3, -3, 1, 10, 6) % POLYNOMIAL(2, 10, -5, -2) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(3, -1, -9, 5, 8) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(3, -1, -9, 5, 8) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(1, 0, 0) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(1, 0, 0) << endl;
cout << POLYNOMIAL(3, 7, 0, -6, -7) / POLYNOMIAL(3, 8, -2, -7, 8) << endl;
cout << POLYNOMIAL(3, 7, 0, -6, -7) % POLYNOMIAL(3, 8, -2, -7, 8) << endl;
cout << POLYNOMIAL(2, -1, -9, 3) / POLYNOMIAL(1, -9, 9) << endl;
cout << POLYNOMIAL(2, -1, -9, 3) % POLYNOMIAL(1, -9, 9) << endl;
cout << POLYNOMIAL(2, 1, 3, -8) / POLYNOMIAL(1, 6, -2) << endl;
cout << POLYNOMIAL(2, 1, 3, -8) % POLYNOMIAL(1, 6, -2) << endl;
cout << POLYNOMIAL(2, -8, 0, -10) / POLYNOMIAL(1, 3, -5) << endl;
cout << POLYNOMIAL(2, -8, 0, -10) % POLYNOMIAL(1, 3, -5) << endl;
cout << POLYNOMIAL(3, 8, 8, -4, -10) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(3, 8, 8, -4, -10) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(2, 10, 6, -3) / POLYNOMIAL(1, 8, -6) << endl;
cout << POLYNOMIAL(2, 10, 6, -3) % POLYNOMIAL(1, 8, -6) << endl;
cout << POLYNOMIAL(2, 0, -1, -8) / POLYNOMIAL(3, -10, -1, 3, 9) << endl;
cout << POLYNOMIAL(2, 0, -1, -8) % POLYNOMIAL(3, -10, -1, 3, 9) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, 4, 5, 3) / POLYNOMIAL(2, 3, 0, 8) << endl;
cout << POLYNOMIAL(2, 4, 5, 3) % POLYNOMIAL(2, 3, 0, 8) << endl;
cout << POLYNOMIAL(1, 0, 5) / POLYNOMIAL(2, -7, 10, -3) << endl;
cout << POLYNOMIAL(1, 0, 5) % POLYNOMIAL(2, -7, 10, -3) << endl;
cout << POLYNOMIAL(2, -4, -7, 2) / POLYNOMIAL(1, 0, 4) << endl;
cout << POLYNOMIAL(2, -4, -7, 2) % POLYNOMIAL(1, 0, 4) << endl;
cout << POLYNOMIAL(3, -2, 10, 9, 8) / POLYNOMIAL(3, -6, 8, 7, 6) << endl;
cout << POLYNOMIAL(3, -2, 10, 9, 8) % POLYNOMIAL(3, -6, 8, 7, 6) << endl;
cout << POLYNOMIAL(1, -10, -1) / POLYNOMIAL(1, 5, 10) << endl;
cout << POLYNOMIAL(1, -10, -1) % POLYNOMIAL(1, 5, 10) << endl;
cout << POLYNOMIAL(2, -2, 0, 10) / POLYNOMIAL(3, 7, -3, 7, 9) << endl;
cout << POLYNOMIAL(2, -2, 0, 10) % POLYNOMIAL(3, 7, -3, 7, 9) << endl;
cout << POLYNOMIAL(3, 0, 9, 6, -6) / POLYNOMIAL(3, 4, -4, 0, 4) << endl;
cout << POLYNOMIAL(3, 0, 9, 6, -6) % POLYNOMIAL(3, 4, -4, 0, 4) << endl;
cout << POLYNOMIAL(2, -6, 1, 8) / POLYNOMIAL(2, 3, 6, 10) << endl;
cout << POLYNOMIAL(2, -6, 1, 8) % POLYNOMIAL(2, 3, 6, 10) << endl;
cout << POLYNOMIAL(3, 3, 3, 2, 0) / POLYNOMIAL(1, 10, -10) << endl;
cout << POLYNOMIAL(3, 3, 3, 2, 0) % POLYNOMIAL(1, 10, -10) << endl;
cout << POLYNOMIAL(0, -6) / POLYNOMIAL(3, 3, -4, 8, -7) << endl;
cout << POLYNOMIAL(0, -6) % POLYNOMIAL(3, 3, -4, 8, -7) << endl;
cout << POLYNOMIAL(2, -2, 10, -3) / POLYNOMIAL(2, 7, -7, 7) << endl;
cout << POLYNOMIAL(2, -2, 10, -3) % POLYNOMIAL(2, 7, -7, 7) << endl;
cout << POLYNOMIAL(1, -10, -9) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(1, -10, -9) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(1, 10, -6) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(1, 10, -6) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(3, 4, -6, 9, -7) / POLYNOMIAL(3, -7, -8, -3, 1) << endl;
cout << POLYNOMIAL(3, 4, -6, 9, -7) % POLYNOMIAL(3, -7, -8, -3, 1) << endl;
cout << POLYNOMIAL(1, 8, 5) / POLYNOMIAL(2, -3, 0, -8) << endl;
cout << POLYNOMIAL(1, 8, 5) % POLYNOMIAL(2, -3, 0, -8) << endl;
cout << POLYNOMIAL(3, -9, 2, 10, 5) / POLYNOMIAL(2, -7, -6, 6) << endl;
cout << POLYNOMIAL(3, -9, 2, 10, 5) % POLYNOMIAL(2, -7, -6, 6) << endl;
cout << POLYNOMIAL(2, 0, -10, 10) / POLYNOMIAL(1, -5, 10) << endl;
cout << POLYNOMIAL(2, 0, -10, 10) % POLYNOMIAL(1, -5, 10) << endl;
cout << POLYNOMIAL(1, -6, 4) / POLYNOMIAL(3, -7, 6, -7, 0) << endl;
cout << POLYNOMIAL(1, -6, 4) % POLYNOMIAL(3, -7, 6, -7, 0) << endl;
cout << POLYNOMIAL(0, -1) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(0, -1) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(2, 9, 9, -1) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, 9, 9, -1) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(2, -1, -8, -6) / POLYNOMIAL(1, 8, 0) << endl;
cout << POLYNOMIAL(2, -1, -8, -6) % POLYNOMIAL(1, 8, 0) << endl;
cout << POLYNOMIAL(3, 6, 1, 0, -1) / POLYNOMIAL(2, 9, -10, 2) << endl;
cout << POLYNOMIAL(3, 6, 1, 0, -1) % POLYNOMIAL(2, 9, -10, 2) << endl;
cout << POLYNOMIAL(1, -5, -9) / POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(1, -5, -9) % POLYNOMIAL(0, 6) << endl;
cout << POLYNOMIAL(0, -3) / POLYNOMIAL(3, -4, -3, -9, 2) << endl;
cout << POLYNOMIAL(0, -3) % POLYNOMIAL(3, -4, -3, -9, 2) << endl;
cout << POLYNOMIAL(3, -4, 2, 3, -1) / POLYNOMIAL(3, -1, -7, -6, 4) << endl;
cout << POLYNOMIAL(3, -4, 2, 3, -1) % POLYNOMIAL(3, -1, -7, -6, 4) << endl;
cout << POLYNOMIAL(3, 1, -10, -6, 0) / POLYNOMIAL(3, -2, 6, 8, 8) << endl;
cout << POLYNOMIAL(3, 1, -10, -6, 0) % POLYNOMIAL(3, -2, 6, 8, 8) << endl;
cout << POLYNOMIAL(1, -2, -6) / POLYNOMIAL(2, 9, -6, -2) << endl;
cout << POLYNOMIAL(1, -2, -6) % POLYNOMIAL(2, 9, -6, -2) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(3, -3, -5, -7, 4) / POLYNOMIAL(3, -1, 1, -1, 7) << endl;
cout << POLYNOMIAL(3, -3, -5, -7, 4) % POLYNOMIAL(3, -1, 1, -1, 7) << endl;
cout << POLYNOMIAL(3, -9, -4, 10, -2) / POLYNOMIAL(3, 3, 9, 4, -4) << endl;
cout << POLYNOMIAL(3, -9, -4, 10, -2) % POLYNOMIAL(3, 3, 9, 4, -4) << endl;
cout << POLYNOMIAL(1, 4, -6) / POLYNOMIAL(3, 3, -1, -3, -6) << endl;
cout << POLYNOMIAL(1, 4, -6) % POLYNOMIAL(3, 3, -1, -3, -6) << endl;
cout << POLYNOMIAL(2, -9, 3, 4) / POLYNOMIAL(2, 7, -8, 8) << endl;
cout << POLYNOMIAL(2, -9, 3, 4) % POLYNOMIAL(2, 7, -8, 8) << endl;
cout << POLYNOMIAL(1, 10, -8) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 10, -8) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 5, -3) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(1, 5, -3) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(2, -10, 4, -6) / POLYNOMIAL(1, -6, -8) << endl;
cout << POLYNOMIAL(2, -10, 4, -6) % POLYNOMIAL(1, -6, -8) << endl;
cout << POLYNOMIAL(1, 4, -6) / POLYNOMIAL(3, 4, 3, -8, -6) << endl;
cout << POLYNOMIAL(1, 4, -6) % POLYNOMIAL(3, 4, 3, -8, -6) << endl;
cout << POLYNOMIAL(2, 7, -10, -3) / POLYNOMIAL(3, 2, -10, -3, 6) << endl;
cout << POLYNOMIAL(2, 7, -10, -3) % POLYNOMIAL(3, 2, -10, -3, 6) << endl;
cout << POLYNOMIAL(1, -1, 0) / POLYNOMIAL(1, 8, 5) << endl;
cout << POLYNOMIAL(1, -1, 0) % POLYNOMIAL(1, 8, 5) << endl;
cout << POLYNOMIAL(3, -8, 2, -10, -2) / POLYNOMIAL(3, 0, 0, 0, 7) << endl;
cout << POLYNOMIAL(3, -8, 2, -10, -2) % POLYNOMIAL(3, 0, 0, 0, 7) << endl;
cout << POLYNOMIAL(3, -8, 4, 7, -2) / POLYNOMIAL(1, -5, 10) << endl;
cout << POLYNOMIAL(3, -8, 4, 7, -2) % POLYNOMIAL(1, -5, 10) << endl;
cout << POLYNOMIAL(3, -3, -10, 5, -6) / POLYNOMIAL(1, -10, 0) << endl;
cout << POLYNOMIAL(3, -3, -10, 5, -6) % POLYNOMIAL(1, -10, 0) << endl;
cout << POLYNOMIAL(2, 1, 1, 1) / POLYNOMIAL(1, 0, 9) << endl;
cout << POLYNOMIAL(2, 1, 1, 1) % POLYNOMIAL(1, 0, 9) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(1, 1, -10) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(1, 1, -10) << endl;
cout << POLYNOMIAL(3, 1, -1, -6, 4) / POLYNOMIAL(1, 1, 7) << endl;
cout << POLYNOMIAL(3, 1, -1, -6, 4) % POLYNOMIAL(1, 1, 7) << endl;
cout << POLYNOMIAL(2, -3, -2, 5) / POLYNOMIAL(2, -6, 5, 5) << endl;
cout << POLYNOMIAL(2, -3, -2, 5) % POLYNOMIAL(2, -6, 5, 5) << endl;
cout << POLYNOMIAL(0, 9) / POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(0, 9) % POLYNOMIAL(0, -6) << endl;
cout << POLYNOMIAL(3, 3, -6, 3, 7) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(3, 3, -6, 3, 7) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(1, 7, 6) / POLYNOMIAL(1, -3, -7) << endl;
cout << POLYNOMIAL(1, 7, 6) % POLYNOMIAL(1, -3, -7) << endl;
cout << POLYNOMIAL(2, 3, -2, 9) / POLYNOMIAL(1, 4, -4) << endl;
cout << POLYNOMIAL(2, 3, -2, 9) % POLYNOMIAL(1, 4, -4) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(3, 1, 8, 1, -8) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(3, 1, 8, 1, -8) << endl;
cout << POLYNOMIAL(1, 8, -3) / POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(1, 8, -3) % POLYNOMIAL(0, 1) << endl;
cout << POLYNOMIAL(2, -6, 5, 4) / POLYNOMIAL(1, 6, -7) << endl;
cout << POLYNOMIAL(2, -6, 5, 4) % POLYNOMIAL(1, 6, -7) << endl;
cout << POLYNOMIAL(1, 5, 9) / POLYNOMIAL(1, -4, -2) << endl;
cout << POLYNOMIAL(1, 5, 9) % POLYNOMIAL(1, -4, -2) << endl;
cout << POLYNOMIAL(1, -9, -4) / POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, -9, -4) % POLYNOMIAL(0, -5) << endl;
cout << POLYNOMIAL(1, 5, -8) / POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(1, 5, -8) % POLYNOMIAL(0, 7) << endl;
cout << POLYNOMIAL(1, 0, -4) / POLYNOMIAL(3, 8, -8, -5, -8) << endl;
cout << POLYNOMIAL(1, 0, -4) % POLYNOMIAL(3, 8, -8, -5, -8) << endl;
cout << POLYNOMIAL(3, -6, 3, 5, 0) / POLYNOMIAL(3, 10, 4, -5, -8) << endl;
cout << POLYNOMIAL(3, -6, 3, 5, 0) % POLYNOMIAL(3, 10, 4, -5, -8) << endl;
cout << POLYNOMIAL(2, -6, 8, -6) / POLYNOMIAL(1, 1, -5) << endl;
cout << POLYNOMIAL(2, -6, 8, -6) % POLYNOMIAL(1, 1, -5) << endl;
cout << POLYNOMIAL(2, -4, -4, 6) / POLYNOMIAL(3, 8, -5, 0, 4) << endl;
cout << POLYNOMIAL(2, -4, -4, 6) % POLYNOMIAL(3, 8, -5, 0, 4) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(2, 10, -8, 7) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(2, 10, -8, 7) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(3, -10, 8, -7, -3) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(3, -10, 8, -7, -3) << endl;
cout << POLYNOMIAL(2, -5, 8, 0) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, -5, 8, 0) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(1, 5, 0) / POLYNOMIAL(2, -4, -10, -6) << endl;
cout << POLYNOMIAL(1, 5, 0) % POLYNOMIAL(2, -4, -10, -6) << endl;
cout << POLYNOMIAL(2, 9, -2, -4) / POLYNOMIAL(1, -8, 6) << endl;
cout << POLYNOMIAL(2, 9, -2, -4) % POLYNOMIAL(1, -8, 6) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(1, -5, -8) << endl;
cout << POLYNOMIAL(3, -6, 7, -7, -8) / POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(3, -6, 7, -7, -8) % POLYNOMIAL(0, -7) << endl;
cout << POLYNOMIAL(3, 8, -2, 6, -2) / POLYNOMIAL(2, -10, 7, 7) << endl;
cout << POLYNOMIAL(3, 8, -2, 6, -2) % POLYNOMIAL(2, -10, 7, 7) << endl;
cout << POLYNOMIAL(3, -3, 5, 4, 2) / POLYNOMIAL(2, -8, -7, -4) << endl;
cout << POLYNOMIAL(3, -3, 5, 4, 2) % POLYNOMIAL(2, -8, -7, -4) << endl;
cout << POLYNOMIAL(2, -7, 10, 8) / POLYNOMIAL(3, 7, 4, -7, 10) << endl;
cout << POLYNOMIAL(2, -7, 10, 8) % POLYNOMIAL(3, 7, 4, -7, 10) << endl;
cout << POLYNOMIAL(3, -5, -3, 4, 8) / POLYNOMIAL(3, 2, 1, -8, 10) << endl;
cout << POLYNOMIAL(3, -5, -3, 4, 8) % POLYNOMIAL(3, 2, 1, -8, 10) << endl;
cout << POLYNOMIAL(1, 4, 3) / POLYNOMIAL(3, -6, -3, 7, 1) << endl;
cout << POLYNOMIAL(1, 4, 3) % POLYNOMIAL(3, -6, -3, 7, 1) << endl;
cout << POLYNOMIAL(0, 1) / POLYNOMIAL(3, 5, 2, -6, -5) << endl;
cout << POLYNOMIAL(0, 1) % POLYNOMIAL(3, 5, 2, -6, -5) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(1, -3, 4) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(1, -3, 4) << endl;
cout << POLYNOMIAL(2, 10, -1, -7) / POLYNOMIAL(3, 3, 10, -9, -2) << endl;
cout << POLYNOMIAL(2, 10, -1, -7) % POLYNOMIAL(3, 3, 10, -9, -2) << endl;
cout << POLYNOMIAL(2, 7, -3, 5) / POLYNOMIAL(3, 8, 9, 6, 3) << endl;
cout << POLYNOMIAL(2, 7, -3, 5) % POLYNOMIAL(3, 8, 9, 6, 3) << endl;
cout << POLYNOMIAL(1, -2, 5) / POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(1, -2, 5) % POLYNOMIAL(0, -4) << endl;
cout << POLYNOMIAL(1, -2, 4) / POLYNOMIAL(1, 4, -6) << endl;
cout << POLYNOMIAL(1, -2, 4) % POLYNOMIAL(1, 4, -6) << endl;
cout << POLYNOMIAL(0, -8) / POLYNOMIAL(3, -2, 4, -4, -6) << endl;
cout << POLYNOMIAL(0, -8) % POLYNOMIAL(3, -2, 4, -4, -6) << endl;
cout << POLYNOMIAL(1, -3, -10) / POLYNOMIAL(1, 4, -2) << endl;
cout << POLYNOMIAL(1, -3, -10) % POLYNOMIAL(1, 4, -2) << endl;
cout << POLYNOMIAL(0, 3) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(0, 3) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(1, 1, 4) / POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(1, 1, 4) % POLYNOMIAL(0, 10) << endl;
cout << POLYNOMIAL(3, -4, 0, 3, -9) / POLYNOMIAL(3, -7, 7, -2, -7) << endl;
cout << POLYNOMIAL(3, -4, 0, 3, -9) % POLYNOMIAL(3, -7, 7, -2, -7) << endl;
cout << POLYNOMIAL(3, 9, 1, 9, 5) / POLYNOMIAL(3, 4, -3, -8, -2) << endl;
cout << POLYNOMIAL(3, 9, 1, 9, 5) % POLYNOMIAL(3, 4, -3, -8, -2) << endl;
cout << POLYNOMIAL(0, -7) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(0, -7) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(1, 9, 2) / POLYNOMIAL(1, -9, 7) << endl;
cout << POLYNOMIAL(1, 9, 2) % POLYNOMIAL(1, -9, 7) << endl;
cout << POLYNOMIAL(2, -7, -7, -4) / POLYNOMIAL(3, 1, 4, 7, -9) << endl;
cout << POLYNOMIAL(2, -7, -7, -4) % POLYNOMIAL(3, 1, 4, 7, -9) << endl;
cout << POLYNOMIAL(3, -8, 3, -10, 7) / POLYNOMIAL(1, 5, 5) << endl;
cout << POLYNOMIAL(3, -8, 3, -10, 7) % POLYNOMIAL(1, 5, 5) << endl;
cout << POLYNOMIAL(3, -8, 10, -3, 4) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(3, -8, 10, -3, 4) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(0, -4) / POLYNOMIAL(1, -1, -6) << endl;
cout << POLYNOMIAL(0, -4) % POLYNOMIAL(1, -1, -6) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(3, -3, -5, 5, -4) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(3, -3, -5, 5, -4) << endl;
cout << POLYNOMIAL(1, 9, 1) / POLYNOMIAL(3, 3, 2, -1, -10) << endl;
cout << POLYNOMIAL(1, 9, 1) % POLYNOMIAL(3, 3, 2, -1, -10) << endl;
cout << POLYNOMIAL(2, 1, 10, 1) / POLYNOMIAL(1, 10, 9) << endl;
cout << POLYNOMIAL(2, 1, 10, 1) % POLYNOMIAL(1, 10, 9) << endl;
cout << POLYNOMIAL(1, 6, 5) / POLYNOMIAL(2, 9, 9, 7) << endl;
cout << POLYNOMIAL(1, 6, 5) % POLYNOMIAL(2, 9, 9, 7) << endl;
cout << POLYNOMIAL(0, -10) / POLYNOMIAL(2, 3, -4, 3) << endl;
cout << POLYNOMIAL(0, -10) % POLYNOMIAL(2, 3, -4, 3) << endl;
cout << POLYNOMIAL(2, 0, 1, 4) / POLYNOMIAL(2, 5, -9, 4) << endl;
cout << POLYNOMIAL(2, 0, 1, 4) % POLYNOMIAL(2, 5, -9, 4) << endl;
cout << POLYNOMIAL(3, 4, -6, -9, 9) / POLYNOMIAL(1, 8, -4) << endl;
cout << POLYNOMIAL(3, 4, -6, -9, 9) % POLYNOMIAL(1, 8, -4) << endl;
cout << POLYNOMIAL(2, -10, -9, 3) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, -10, -9, 3) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, -1, -1, -10) / POLYNOMIAL(1, 2, -10) << endl;
cout << POLYNOMIAL(2, -1, -1, -10) % POLYNOMIAL(1, 2, -10) << endl;
cout << POLYNOMIAL(0, 10) / POLYNOMIAL(3, -4, -10, 4, -5) << endl;
cout << POLYNOMIAL(0, 10) % POLYNOMIAL(3, -4, -10, 4, -5) << endl;
cout << POLYNOMIAL(0, 8) / POLYNOMIAL(3, 2, -10, 7, -10) << endl;
cout << POLYNOMIAL(0, 8) % POLYNOMIAL(3, 2, -10, 7, -10) << endl;
cout << POLYNOMIAL(2, -6, 3, -1) / POLYNOMIAL(1, 3, -3) << endl;
cout << POLYNOMIAL(2, -6, 3, -1) % POLYNOMIAL(1, 3, -3) << endl;
cout << POLYNOMIAL(2, 4, 9, 3) / POLYNOMIAL(2, -5, -5, 2) << endl;
cout << POLYNOMIAL(2, 4, 9, 3) % POLYNOMIAL(2, -5, -5, 2) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(1, 5, -5) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(1, 5, -5) << endl;
cout << POLYNOMIAL(1, -4, -1) / POLYNOMIAL(3, 4, 3, -10, -2) << endl;
cout << POLYNOMIAL(1, -4, -1) % POLYNOMIAL(3, 4, 3, -10, -2) << endl;
cout << POLYNOMIAL(3, -5, -10, -1, 9) / POLYNOMIAL(3, -6, 6, -4, 9) << endl;
cout << POLYNOMIAL(3, -5, -10, -1, 9) % POLYNOMIAL(3, -6, 6, -4, 9) << endl;
cout << POLYNOMIAL(2, 1, 6, -7) / POLYNOMIAL(1, 3, -2) << endl;
cout << POLYNOMIAL(2, 1, 6, -7) % POLYNOMIAL(1, 3, -2) << endl;
cout << POLYNOMIAL(1, -6, 0) / POLYNOMIAL(1, 9, -4) << endl;
cout << POLYNOMIAL(1, -6, 0) % POLYNOMIAL(1, 9, -4) << endl;
cout << POLYNOMIAL(1, 2, -9) / POLYNOMIAL(1, -1, -6) << endl;
cout << POLYNOMIAL(1, 2, -9) % POLYNOMIAL(1, -1, -6) << endl;
cout << POLYNOMIAL(3, 2, -9, 2, 0) / POLYNOMIAL(2, 3, 8, 9) << endl;
cout << POLYNOMIAL(3, 2, -9, 2, 0) % POLYNOMIAL(2, 3, 8, 9) << endl;
cout << POLYNOMIAL(2, 4, 8, -2) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(2, 4, 8, -2) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(1, -9, 4) / POLYNOMIAL(1, -1, 0) << endl;
cout << POLYNOMIAL(1, -9, 4) % POLYNOMIAL(1, -1, 0) << endl;
cout << POLYNOMIAL(1, -9, -10) / POLYNOMIAL(2, 5, -10, -5) << endl;
cout << POLYNOMIAL(1, -9, -10) % POLYNOMIAL(2, 5, -10, -5) << endl;
cout << POLYNOMIAL(3, -10, -1, -2, -6) / POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(3, -10, -1, -2, -6) % POLYNOMIAL(0, -10) << endl;
cout << POLYNOMIAL(2, 2, 4, -3) / POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(2, 2, 4, -3) % POLYNOMIAL(0, 4) << endl;
cout << POLYNOMIAL(3, -10, -5, -2, 4) / POLYNOMIAL(3, -5, -10, 7, -6) << endl;
cout << POLYNOMIAL(3, -10, -5, -2, 4) % POLYNOMIAL(3, -5, -10, 7, -6) << endl;
cout << POLYNOMIAL(3, 3, 3, 9, 8) / POLYNOMIAL(3, -8, 4, 5, 1) << endl;
cout << POLYNOMIAL(3, 3, 3, 9, 8) % POLYNOMIAL(3, -8, 4, 5, 1) << endl;
cout << POLYNOMIAL(3, -9, -2, 0, -9) / POLYNOMIAL(3, 9, 7, -1, 2) << endl;
cout << POLYNOMIAL(3, -9, -2, 0, -9) % POLYNOMIAL(3, 9, 7, -1, 2) << endl;
cout << POLYNOMIAL(2, 4, -1, -2) / POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(2, 4, -1, -2) % POLYNOMIAL(0, -1) << endl;
cout << POLYNOMIAL(1, 10, 10) / POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(1, 10, 10) % POLYNOMIAL(0, 2) << endl;
cout << POLYNOMIAL(2, 0, 3, -8) / POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(2, 0, 3, -8) % POLYNOMIAL(0, -2) << endl;
cout << POLYNOMIAL(0, -9) / POLYNOMIAL(2, 3, 8, -6) << endl;
cout << POLYNOMIAL(0, -9) % POLYNOMIAL(2, 3, 8, -6) << endl;
cout << POLYNOMIAL(3, 3, 8, -3, -9) / POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(3, 3, 8, -3, -9) % POLYNOMIAL(0, -3) << endl;
cout << POLYNOMIAL(2, 4, -6, 7) / POLYNOMIAL(3, -10, 0, 1, -2) << endl;
cout << POLYNOMIAL(2, 4, -6, 7) % POLYNOMIAL(3, -10, 0, 1, -2) << endl;
cout << POLYNOMIAL(3, -7, -1, -6, -8) / POLYNOMIAL(1, -2, -7) << endl;
cout << POLYNOMIAL(3, -7, -1, -6, -8) % POLYNOMIAL(1, -2, -7) << endl;
cout << POLYNOMIAL(0, 2) / POLYNOMIAL(2, 2, -3, 7) << endl;
cout << POLYNOMIAL(0, 2) % POLYNOMIAL(2, 2, -3, 7) << endl;
cout << POLYNOMIAL(2, -10, -1, -1) / POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(2, -10, -1, -1) % POLYNOMIAL(0, 0) << endl;
cout << POLYNOMIAL(3, -2, -8, -7, -7) / POLYNOMIAL(2, 6, -2, 2) << endl;
cout << POLYNOMIAL(3, -2, -8, -7, -7) % POLYNOMIAL(2, 6, -2, 2) << endl;
cout << POLYNOMIAL(3, 2, -8, -7, 0) / POLYNOMIAL(3, 8, 7, -10, 9) << endl;
cout << POLYNOMIAL(3, 2, -8, -7, 0) % POLYNOMIAL(3, 8, 7, -10, 9) << endl;
cout << POLYNOMIAL(2, 5, -3, -4) / POLYNOMIAL(2, -10, -3, 4) << endl;
cout << POLYNOMIAL(2, 5, -3, -4) % POLYNOMIAL(2, -10, -3, 4) << endl;
cout << POLYNOMIAL(0, 0) / POLYNOMIAL(3, 10, 8, 10, -2) << endl;
cout << POLYNOMIAL(0, 0) % POLYNOMIAL(3, 10, 8, 10, -2) << endl;
cout << POLYNOMIAL(2, -3, 8, -3) / POLYNOMIAL(2, -7, 0, -2) << endl;
cout << POLYNOMIAL(2, -3, 8, -3) % POLYNOMIAL(2, -7, 0, -2) << endl;
cout << POLYNOMIAL(1, -3, -10) / POLYNOMIAL(1, 5, -8) << endl;
cout << POLYNOMIAL(1, -3, -10) % POLYNOMIAL(1, 5, -8) << endl;
cout << POLYNOMIAL(2, -4, 5, 6) / POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(2, -4, 5, 6) % POLYNOMIAL(0, 5) << endl;
cout << POLYNOMIAL(1, -7, 10) / POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(1, -7, 10) % POLYNOMIAL(0, 3) << endl;
cout << POLYNOMIAL(1, 7, 6) / POLYNOMIAL(1, -8, -2) << endl;
cout << POLYNOMIAL(1, 7, 6) % POLYNOMIAL(1, -8, -2) << endl;
cout << POLYNOMIAL(2, 3, 5, -4) / POLYNOMIAL(1, 6, -8) << endl;
cout << POLYNOMIAL(2, 3, 5, -4) % POLYNOMIAL(1, 6, -8) << endl;
cout << POLYNOMIAL(3, -5, -7, 5, -1) / POLYNOMIAL(1, -7, -3) << endl;
cout << POLYNOMIAL(3, -5, -7, 5, -1) % POLYNOMIAL(1, -7, -3) << endl;
cout << POLYNOMIAL(2, 2, 10, -9) / POLYNOMIAL(3, -1, 4, -1, 2) << endl;
cout << POLYNOMIAL(2, 2, 10, -9) % POLYNOMIAL(3, -1, 4, -1, 2) << endl;
cout << POLYNOMIAL(3, -10, -10, -7, 4) / POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, -10, -10, -7, 4) % POLYNOMIAL(0, -8) << endl;
cout << POLYNOMIAL(3, 2, 10, 6, 8) / POLYNOMIAL(1, -2, 7) << endl;
cout << POLYNOMIAL(3, 2, 10, 6, 8) % POLYNOMIAL(1, -2, 7) << endl;
cout << POLYNOMIAL(3, 6, 6, 1, -9) / POLYNOMIAL(3, 9, 2, -7, 3) << endl;
cout << POLYNOMIAL(3, 6, 6, 1, -9) % POLYNOMIAL(3, 9, 2, -7, 3) << endl;
cout << POLYNOMIAL(3, -6, -8, -4, 0) / POLYNOMIAL(2, -2, -4, -6) << endl;
cout << POLYNOMIAL(3, -6, -8, -4, 0) % POLYNOMIAL(2, -2, -4, -6) << endl;
cout << POLYNOMIAL(0, 7) / POLYNOMIAL(1, 1, 0) << endl;
cout << POLYNOMIAL(0, 7) % POLYNOMIAL(1, 1, 0) << endl;
cout << POLYNOMIAL(1, 7, -6) / POLYNOMIAL(1, -6, 1) << endl;
cout << POLYNOMIAL(1, 7, -6) % POLYNOMIAL(1, -6, 1) << endl;
cout << POLYNOMIAL(1, 0, 10) / POLYNOMIAL(0, 9) << endl;
cout << POLYNOMIAL(1, 0, 10) % POLYNOMIAL(0, 9) << endl;

return 0;
}
