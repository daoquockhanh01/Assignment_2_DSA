#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

//SYMBOL TABLE (CẤU TRÚC SPLAY TREE)
class SymbolTable {
public:
    class Node {
    public:
        string val;
        int block;
        Node* next;
    public:
        Node() {
            this->val = "";
            this->block = 0;
            this->next = NULL;
        }
        Node(string val, int block) {
            this->val = val;
            this->block = block;
            this->next = NULL;
        }
    };
    class LL {
    public:
        Node* head;
        Node* tail;
        int size;
    public:
        LL() {
            this->head = NULL;
            this->tail = NULL;
            this->size = 0;
        }
        void add(string name, int block) {
            Node* tmp = new Node(name, block);
            if (this->head == NULL) {
                this->head = tmp;
                this->tail = tmp;
            }
            else {
                this->tail->next = tmp;
                this->tail = tmp;
            }
            this->size++;
        }
        void remove(Node* node) {
            if (this->head == NULL) return;
            if (this->size == 1) {
                if (this->head != node) return;
                else {
                    this->head = NULL;
                    this->tail = NULL;
                }
            }
            else {
                if (node == this->head) 
                    this->head = this->head->next;
                else if (node == this->tail) {
                    Node* run = this->head;
                    for (int i = 0; i < this->size - 2; i++)
                        run = run->next;
                    this->tail = run;
                    run->next = NULL;
                }
                else {
                    Node* run = this->head;
                    while (run->next != node)
                        run = run->next;
                    Node* sav = run->next;
                    run->next = sav->next;
                }
            }
            delete node;
            this->size--;
        }
        void clear() {
            if (this->head == NULL) return;
            Node* run = this->head;
            while (run) {
                Node* sav = run;
                run = run->next;
                delete sav;
            }
            this->head = NULL;
            this->tail = NULL;
            this->size = 0;
        }
        ~LL() {
            clear();
        }
    };

public:
    //SYMBOL (PHẦN TỬ CỦA SPLAY TREE)
    class Symbol {
    public:
        string id;          // Tên danh hiệu
        string type;        // Kiểu dữ liệu hoặc thông tin của hàm
        int block;          // Mức khối của phần tử
        string value;       // Giá trị nắm giữ
        Symbol* left;
        Symbol* right;
        Symbol* parent;

    public:
        Symbol() {
            this->id = this->type = "";
            this->block = 0;
            this->value = "";
            this->left = this->right = this->parent = NULL;
        }
        Symbol(string id, string type, int block = 0, string value = "") {
            this->id = id;
            this->type = type;
            this->block = block;
            this->value = value;
            this->left = this->right = this->parent = NULL;
        }
    };

private:
    //******************************************************
    //ATTRIBUTES OF SYMBOL TABLE****************************
    //******************************************************
    Symbol* root;
    LL* lK;
    int cBlock;

public:
    //******************************************************
    //CONSTRUCTOR AND SUPPORT FUNCTIONS*********************
    //******************************************************
    SymbolTable() {
        this->root = NULL;
        lK = new LL();
        this->cBlock = 0;
    }
    void run(string filename);
    int cp(string a, string b) {
        if (a == b) return 0;
        int pos = 0;
        while (a[pos] != '\n' && b[pos] != '\n') {
            if ((int)a[pos] > (int)b[pos]) return 1;
            else if ((int)a[pos] < (int)b[pos]) return -1;
            else pos++;
        }
        if (a.size() < b.size()) return -1;
        return 1;
    }
    int comp(Symbol* a, Symbol* b) {
        if (a->block > b->block) return 1;
        else if (a->block < b->block) return -1;
        else if (a->block == b->block) return cp(a->id, b->id);
        else return 0;
    }
    int countSpace(string s) {
        int ans = 0;
        bool done = true;
        for (unsigned int i = 0; i < s.size(); i++) {
            if (s[i] == '(') done = false;
            if (s[i] == ')') done = true;
            if (s[i] == ' ')
                if (done == true) ans++;
        }
        return ans;
    }
    int countComma(string s) {
        int ans = 0;
        for (unsigned int i = 0; i < s.size(); i++)
            if (s[i] == ',') ans++;
        return ans;
    }
    void divideString(string s, string& code, string& name, string& type, string& stat) {
        int space = countSpace(s), pos = 0;
        bool done = true;
        while (s[pos] != ' ' && s[pos] != '\n') {
            code += s[pos];
            pos++;
            if ((unsigned int)pos > s.size() - 1) break;
        }
        if (space >= 1) {
            pos = 0;
            s.erase(0, code.size() + 1);
            while (s[pos] != ' ' && s[pos] != '\n') {
                name += s[pos];
                pos++;
                if ((unsigned int)pos > s.size() - 1) break;
            }
        }
        if (space >= 2) {
            pos = 0;
            s.erase(0, name.size() + 1);
            while (s[pos] != '\n') {
                if (s[pos] == ' ')
                    if (done == true) break;
                if (s[pos] == '(') done = false;
                if (s[pos] == ')') done = true;
                type += s[pos];
                pos++;
                if ((unsigned int)pos > s.size() - 1) break;
            }
        }
        if (space >= 3) {
            pos = 0;
            s.erase(0, type.size() + 1);
            while (s[pos] != ' ' && s[pos] != '\n') {
                stat += s[pos];
                pos++;
                if ((unsigned int)pos > s.size() - 1) break;
            }
        }
    }
    string returnValue(string s) {
        string str = "";
        unsigned int pos = 0;
        while (s[pos] != '>') {
            str += s[pos];
            pos++;
        }
        s.erase(0, str.size() + 1);
        return s;
    }
    string paraValue(string s) {
        string str = "";
        unsigned int pos = 1;
        while (s[pos] != '-') {
            str += s[pos];
            pos++;
        }
        return str;
    }

    //******************************************************
    //BASIC METHODS OF SPLAY TREE***************************
    //******************************************************
    Symbol* rotateRight(Symbol* node) {
        if (node == NULL || node->left == NULL) return node;
        Symbol* sav = node->parent, * l = node->left;
        node->left = l->right;
        if (l->right != NULL) l->right->parent = node;
        l->right = node;
        node->parent = l;
        l->parent = sav;
        if (sav != NULL) {
            if (node == sav->left) sav->left = l;
            else if (node == sav->right) sav->right = l;
        }
        return l;
    }
    Symbol* rotateLeft(Symbol* node) {
        if (node == NULL || node->right == NULL) return node;
        Symbol* sav = node->parent, * r = node->right;
        node->right = r->left;
        if (r->left != NULL) r->left->parent = node;
        r->left = node;
        node->parent = r;
        r->parent = sav;
        if (sav != NULL) {
            if (node == sav->left) sav->left = r;
            else if (node == sav->right) sav->right = r;
        }
        return r;
    }
    Symbol* splay(Symbol* tmp, int& num_splay) {
        if (tmp == NULL || tmp->parent == NULL) return tmp;
        while (tmp->parent != NULL) {
            // ZIG AND ZAG
            if (tmp->parent->parent == NULL) {
                // ZIG
                if (tmp == tmp->parent->left)
                    tmp = rotateRight(tmp->parent);
                // ZAG
                else if (tmp == tmp->parent->right)
                    tmp = rotateLeft(tmp->parent);
            }
            // Z*G - Z*G
            else {
                // ZIG - Z*G
                if (tmp->parent == tmp->parent->parent->left) {
                    // ZIG - ZIG
                    if (tmp == tmp->parent->left)
                        tmp->parent = rotateRight(tmp->parent->parent);
                    else if (tmp == tmp->parent->right)
                        tmp = rotateLeft(tmp->parent);
                    tmp = rotateRight(tmp->parent);
                }
                //  ZAG - Z*G
                else if (tmp->parent == tmp->parent->parent->right) {
                    // ZAG - ZAG
                    if (tmp == tmp->parent->right)
                        tmp->parent = rotateLeft(tmp->parent->parent);
                    else if (tmp == tmp->parent->left)
                        tmp = rotateRight(tmp->parent);
                    tmp = rotateLeft(tmp->parent);
                }
            }
        }
        num_splay++;
        return tmp;
    }
    Symbol* splay(Symbol* tmp) {
        if (tmp == NULL || tmp->parent == NULL) return tmp;
        while (tmp->parent != NULL) {
            // ZIG AND ZAG
            if (tmp->parent->parent == NULL) {
                // ZIG
                if (tmp == tmp->parent->left)
                    tmp = rotateRight(tmp->parent);
                // ZAG
                else if (tmp == tmp->parent->right)
                    tmp = rotateLeft(tmp->parent);
            }
            // Z*G - Z*G
            else {
                // ZIG - Z*G
                if (tmp->parent == tmp->parent->parent->left) {
                    // ZIG - ZIG
                    if (tmp == tmp->parent->left)
                        tmp->parent = rotateRight(tmp->parent->parent);
                    else if (tmp == tmp->parent->right)
                        tmp = rotateLeft(tmp->parent);
                    tmp = rotateRight(tmp->parent);
                }
                else if (tmp->parent == tmp->parent->parent->right) {
                    // ZAG - ZAG
                    if (tmp == tmp->parent->right)
                        tmp->parent = rotateLeft(tmp->parent->parent);
                    else if (tmp == tmp->parent->left)
                        tmp = rotateRight(tmp->parent);
                    tmp = rotateLeft(tmp->parent);
                }
            }
        }
        return tmp;
    }
    //TÌM KIẾM MỘT DANH HIỆU TRONG CÂY (KHÔNG SPLAY SAU KHI SEARCH)
    Symbol* search_not_splay(string name, int block, int& num_comp) {
        if (this->root == NULL) return NULL;
        Symbol* run = this->root;
        Symbol* sav = new Symbol(name, "string", block);
        while (run) {
            int c = comp(sav, run);
            num_comp++;
            if (c == 0) {
                delete sav;
                return run;
            }
            else if (c == -1) run = run->left;
            else if (c == 1) run = run->right;
        }
        delete sav;
        return run;
    }
    Symbol* search_not_splay(string name, int block) {
        if (this->root == NULL) return NULL;
        Symbol* run = this->root;
        Symbol* sav = new Symbol(name, "string", block);
        while (run) {
            int c = comp(sav, run);
            if (c == 0) {
                delete sav;
                return run;
            }
            else if (c == -1) run = run->left;
            else if (c == 1) run = run->right;
        }
        delete sav;
        return run;
    }
    //TÌM KIẾM MỘT DANH HIỆU TRONG CÂY (SPLAY SAU KHI SEARCH)
    Symbol* search_and_splay(string name, int block, int& num_comp, int& num_splay) {
        if (this->root == NULL) return NULL;
        Symbol* run = this->root;
        Symbol* sav = new Symbol(name, "string", block);
        if (comp(sav, this->root) == 0) {
            num_comp++;
            delete sav;
            return this->root;
        }
        while (run) {
            int c = comp(sav, run);
            num_comp++;
            if (c == 0) {
                this->root = SymbolTable::splay(run, num_splay);
                run = this->root;
                delete sav;
                return run;
            }
            else if (c == -1) run = run->left;
            else if (c == 1) run = run->right;
            else break;
        }
        if (run == NULL) return run;
        run = SymbolTable::splay(run, num_splay);
        delete sav;
        return run;
    }
    Symbol* search_and_splay(string name, int block) {
        if (this->root == NULL) return NULL;
        Symbol* run = this->root;
        Symbol* sav = new Symbol(name, "string", block);
        if (comp(sav, this->root) == 0) {
            delete sav;
            return this->root;
        }
        while (run) {
            int c = comp(sav, run);
            if (c == 0) {
                this->root = SymbolTable::splay(run);
                run = this->root;
                delete sav;
                return run;
            }
            else if (c == -1) run = run->left;
            else run = run->right;
        }
        if (run == NULL) return run;
        int num_splay = 0;
        this->root = splay(run, num_splay);
        delete sav;
        return run;
    }
    void insert(string names, string types, int blck, int& num_comp, int& num_splay) {
        Symbol* tmp = new Symbol(names, types, blck);
        if (this->root == NULL)
            this->root = tmp;
        else {
            Symbol* run = this->root;
            bool done = false;
            while (done == false) {
                if (comp(tmp, run) == -1) {
                    num_comp++;
                    if (run->left != NULL) run = run->left;
                    else done = true;
                }
                else {
                    num_comp++;
                    if (run->right != NULL) run = run->right;
                    else done = true;
                }
            }
            if (comp(tmp, run) == -1) {
                run->left = tmp;
                tmp->parent = run;
            }
            else {
                run->right = tmp;
                tmp->parent = run;
            }
        }
        this->root = splay(tmp, num_splay);
    }
    //KIỂM TRA SỰ TỒN TẠI CỦA MỘT BIẾN (HOẶC HÀM) TRONG MỘT BLOCK NHẤT ĐỊNH
    bool containName(string name, int gBlock) {
        if (this->root == NULL) return false;
        Symbol* run = this->root;
        Symbol* sav = new Symbol(name, "string", gBlock);
        while (run) {
            int c = comp(sav, run);
            if (c == 0) {
                delete sav;
                return true;
            }
            else if (c == -1) run = run->left;
            else run = run->right;
        }
        delete sav;
        return false;
    }
    Symbol* maxElement(Symbol* node) {
        if (node == NULL) return NULL;
        if (node->right == NULL) return node;
        Symbol* run = node;
        while (run->right != NULL)
            run = run->right;
        return run;
    }
    void printPre(Symbol* node, string& ans) {
        if (node == NULL) return;
        ans += node->id;
        ans += "//";
        ans += to_string(node->block);
        if (node->left != NULL) {
            ans += " ";
            printPre(node->left, ans);
        }
        if (node->right != NULL) {
            ans += " ";
            printPre(node->right, ans);
        }
    }
    void remove(string name, int gBlock) {
        if (this->root == NULL) return;
        Symbol* ptr = SymbolTable::search_and_splay(name, gBlock);
        if (ptr == NULL) return;
        if (this->root->left == NULL) {
            if (this->root->right != NULL) this->root->right->parent = NULL;
            this->root = this->root->right;
        }
        else {
            Symbol* maxLeft = SymbolTable::maxElement(this->root->left);
            this->root->left->parent = NULL;
            this->root->left = SymbolTable::splay(maxLeft);
            this->root->left->right = this->root->right;
            if (this->root->right != NULL) this->root->right->parent = this->root->left;
            this->root = this->root->left;
        }
        delete ptr;
    }
    void clearRec(Symbol* node) {
        if (node == NULL) return;
        if (node->left != NULL) clearRec(node->left);
        if (node->right != NULL) clearRec(node->right);
        delete node;
        node = NULL;
    }
    void clearK() {
        SymbolTable::clearRec(this->root);
        this->root = NULL;
        this->cBlock = 0;
    }

    //******************************************************
    //REQUIRED FUNCTIONS************************************
    //******************************************************
    //CHECK THE INSTRUCTION OF NAME
    bool checkName(string s) {
        if (s[0] < 'a' || s[0] > 'z') return false;
        for (unsigned int i = 0; i < s.size(); i++)
            if ((s[i] < 'a' || s[i] > 'z') && (s[i] < 'A' || s[i] > 'Z') && (s[i] < '0' || s[i] > '9') && s[i] != '_')
                return false;
        return true;
    }
    //CHECK THE INSTRUCTION OF FUNCTION
    bool checkFunction(string s) {
        if (s[0] != '(') return false;
        s.erase(0, 1);
        int pos = 0;
        for (unsigned int i = 0; i < s.size(); i++)
            if (s[i] == ')') pos++;
        if (pos != 1) return false;
        string str = "";
        pos = 0;
        while (s[pos] != ')') {
            if (s[0] == ',') s.erase(0, 1);
            while (s[pos] != ',' && s[pos] != ')') {
                str += s[pos];
                pos++;
            }
            if (str != "number" && str != "string") return false;
            s.erase(0, str.size());
            pos = 0;
            str = "";
        }
        s.erase(0, 1);
        if (s.size() < 2) return false;
        if (s[0] != '-' || s[1] != '>') return false;
        s.erase(0, 2);
        if (s != "number" && s != "string") return false;
        return true;
    }
    //CHECK THE VALUE TYPE NUMBER
    bool checkNumber(string typ) {
        for (unsigned int i = 0; i < typ.size(); i++)
            if (typ[i] < '0' || typ[i] > '9')
                return false;
        return true;
    }
    //CHECK THE VALUE TYPE STRING
    bool checkString(string typ) {
        if (typ[0] != char(39) || typ[typ.size() - 1] != char(39)) return false;
        for (unsigned int i = 1; i < typ.size() - 1; i++)
            if ((typ[i] < '0' || typ[i] > '9') && (typ[i] < 'a' || typ[i] > 'z') && (typ[i] < 'A' || typ[i] > 'Z') && typ[i] != ' ')
                return false;
        return true;
    }
    //CHECK THE INSTRUCION OF LINE
    void checkInstruction(string s) {
        int space = countSpace(s);
        string code, name, typ, stat;
        divideString(s, code, name, typ, stat);
        //CHECK CODE
        if (code != "INSERT" && code != "ASSIGN" && code != "BEGIN" && code != "END" && code != "LOOKUP" && code != "PRINT")
            throw InvalidInstruction(s);
        //CHECK NAME
        if (space >= 1) {
            if (code != "INSERT" && code != "ASSIGN" && code != "LOOKUP") 
                throw InvalidInstruction(s);
            if (checkName(name) == false)
                throw InvalidInstruction(s);
        }
        //CHECK TYPE AND VALUE
        if (space >= 2) {
            if (code != "INSERT" && code != "ASSIGN") throw InvalidInstruction(s);
            //CHECK TYPE
            if (code == "INSERT") {
                if (typ != "number" && typ != "string" && checkFunction(typ) == false)
                    throw InvalidInstruction(s);
            }
            //CHECK VALUE
            else if (code == "ASSIGN") {
                //CONST NUMBER
                if (typ[0] >= '0' && typ[0] <= '9') {
                    if (checkNumber(typ) == false)
                        throw InvalidInstruction(s);
                }
                //CONST STRING
                else if (typ[0] == char(39)) {
                    if (checkString(typ) == false)
                        throw InvalidInstruction(s);
                }
                //ID AND ID_FUNCTION
                else {
                    int c1 = 0, c2 = 0;
                    for (unsigned int i = 0; i < typ.size(); i++) {
                        if (typ[i] == '(') c1++;
                        else if (typ[i] == ')') c2++;
                        else continue;
                    }
                    //NẾU VALUE LÀ TÊN MỘT DANH HIỆU KHÁC
                    if (c1 == 0 && c2 == 0) {
                        if (checkName(typ) == false)
                            throw InvalidInstruction(s);
                    }
                    //NẾU VALUE GỌI ĐẾN MỘT HÀM KHÁC
                    else if (c1 == 1 && c2 == 1) {
                        string sv = "";
                        int index = 0;
                        while (typ[index] != '(') {
                            sv += typ[index];
                            index++;
                        }
                        if (checkName(sv) == false)
                            throw InvalidInstruction(s);
                        typ.erase(0, sv.size());
                        if (typ[0] != '(' || typ[typ.size() - 1] != ')')
                            throw InvalidInstruction(s);
                        typ.erase(0, 1);
                        int pos = 0;
                        string str = "";
                        // 
                        while (typ[pos] != ')') {
                            if (typ[0] == ',') typ.erase(0, 1);
                            while (typ[pos] != ',' && typ[pos] != ')') {
                                str += typ[pos];
                                pos++;
                            }
                            if (checkName(str) == false && checkNumber(str) == false && checkString(str) == false)
                                throw InvalidInstruction(s);
                            typ.erase(0, str.size());
                            pos = 0;
                            str = "";
                        }
                    }
                    else throw InvalidInstruction(s);
                }
            }
        }
        //CHECK STAT
        if (space >= 3) {
            if (code != "INSERT") throw InvalidInstruction(s);
            if (stat != "true" && stat != "false")
                throw InvalidInstruction(s);
        }
    }
    //INSERT
    void SymbolInsert(string s) {
        string code, name, typ, stat;
        divideString(s, code, name, typ, stat);
        if (checkFunction(typ) == true)
            if (stat == "false")
                if (this->cBlock != 0)
                    throw InvalidDeclaration(s);
        int num_comp = 0, num_splay = 0;
        if (stat == "true") {
            if (containName(name, 0) == true)
                throw Redeclared(s);
            SymbolTable::insert(name, typ, 0, num_comp, num_splay);
            lK->add(name, 0);
        }
        else {
            if (containName(name, this->cBlock) == true)
                throw Redeclared(s);
            SymbolTable::insert(name, typ, this->cBlock, num_comp, num_splay);
            lK->add(name, this->cBlock);
        }
        cout << num_comp << " " << num_splay;
    }
    //ASSIGN
    void SymbolAssign(string s) {
        int num_comp = 0, num_splay = 0;
        string code, name, typ, stat;
        divideString(s, code, name, typ, stat);
        string code2 = code, name2 = name, typ2 = typ;
        //XỬ LÝ GÁN BIẾN VÀ HÀM
        if ((typ[0] < '0' || typ[0] > '9') && typ[0] != char(39)) {
            int isFunc = 0;
            string str = "";
            for (unsigned int i = 0; i < typ.size(); i++) {
                if (typ[i] == '(') {
                    isFunc++;
                    break;
                }
                str += typ[i];
            }
            //NẾU LÀ HÀM
            if (isFunc == 1) {
                typ.erase(0, str.size() + 1);
                //str = sum, typ = 1,1)
                Symbol* f = search_and_splay(str, 0, num_comp, num_splay);
                if (f == NULL)
                    throw Undeclared(s);
                if (f->type == "string" || f->type == "number")
                    throw TypeMismatch(s);
                if (countComma(typ) != countComma(f->type))
                    throw TypeMismatch(s);
                string para = paraValue(f->type), ret = returnValue(f->type);
                //para = number,number), ret = number
                //bắt đầu kiểm tra xử lý xem các tham số truyền vào và kiểu dữ liệu có khớp nhau không
                while (typ[0] != ')') {
                    string c1 = "", c2 = "";
                    int pos1 = 0, pos2 = 0;
                    while (typ[pos1] != ',' && typ[pos1] != ')') {
                        c1 += typ[pos1];
                        pos1++;
                    }
                    typ.erase(0, c1.size());
                    if (typ[0] == ',') typ.erase(0, 1);
                    while (para[pos2] != ',' && para[pos2] != ')') {
                        c2 += para[pos2];
                        pos2++;
                    }
                    para.erase(0, c2.size());
                    if (para[0] == ',') para.erase(0, 1);
                    if (c2 == "number") {
                        //c1 là chuỗi
                        if (checkString(c1) == true)
                            throw TypeMismatch(s);
                        //c1 là số --> đúng --> không làm gì
                        //c1 là danh hiệu
                        if (checkNumber(c1) == false) {
                            int fBlock = this->cBlock;
                            Symbol* ptr = NULL;
                            bool done = false;
                            while (fBlock >= 0) {
                                int num_comp2 = num_comp, num_splay2 = num_splay;
                                ptr = search_and_splay(c1, fBlock, num_comp2, num_splay2);
                                if (ptr != NULL) {
                                    done = true;
                                    if (ptr->type == "number") {
                                        num_comp = num_comp2;
                                        num_splay = num_splay2;
                                        break;
                                    }
                                }
                                fBlock--;
                            }
                            if (ptr == NULL || ptr->type != "number") {
                                if (done == false) throw Undeclared(s);
                                else if (done == true) throw TypeMismatch(s);
                            }
                        }
                    }
                    else if (c2 == "string") {
                        //c1 là số
                        if (checkNumber(c1) == true)
                            throw TypeMismatch(s);
                        //c1 là chuỗi --> đúng --> không làm gì
                        //c1 là danh hiệu
                        if (checkString(c1) == false) {
                            int fBlock = this->cBlock;
                            Symbol* ptr = NULL;
                            bool done = false;
                            while (fBlock >= 0) {
                                int num_comp2 = num_comp, num_splay2 = num_splay;
                                ptr = search_and_splay(c1, fBlock, num_comp2, num_splay2);
                                if (ptr != NULL) {
                                    done = true;
                                    if (ptr->type == "string") {
                                        num_comp = num_comp2;
                                        num_splay = num_splay2;
                                        break;
                                    }
                                }
                                fBlock--;
                            }
                            if (ptr == NULL || ptr->type != "string") {
                                if (done == false) throw Undeclared(s);
                                else if (done == true) throw TypeMismatch(s);
                            }
                        }
                    }
                }
                //đã kiểm tra xong phần tham số truyền vào là ổn
                //bây giờ sẽ gán giá trị cho name
                int gBlock = this->cBlock;
                bool done = false;
                Symbol* k = NULL;
                while (gBlock >= 0) {
                    int num_comp2 = num_comp, num_splay2 = num_splay;
                    k = search_and_splay(name, gBlock, num_comp2, num_splay2);
                    if (k != NULL) {
                        done = true;
                        if (k->type == ret) {
                            num_comp = num_comp2;
                            num_splay = num_splay2;
                            break;
                        }
                    }
                    gBlock--;
                }
                if (k == NULL || k->type != ret) {
                    if (done == false) throw Undeclared(s);
                    else if (done == true) throw TypeMismatch(s);
                }
                else k->value = typ2;
            }
            //NẾU LÀ BIẾN
            else if (isFunc == 0) {
                int gBlock = this->cBlock;
                Symbol* gan = this->root;
                while (gBlock >= 0) {
                    int num_comp2 = num_comp, num_splay2 = num_splay;
                    gan = search_and_splay(str, gBlock, num_comp2, num_splay2);
                    if (gan != NULL) {
                        num_comp = num_comp2;
                        num_splay = num_splay2;
                        break;
                    }
                    gBlock--;
                }
                if (gan == NULL) throw Undeclared(s);
                if (gan->type != "string" && gan->type != "number") throw TypeMismatch(s);
                //đã tìm được biến gán
                //kiểm tra biến được gán
                gBlock = this->cBlock;
                bool done = false;
                Symbol* k = this->root;
                while (gBlock >= 0) {
                    int num_comp2 = num_comp, num_splay2 = num_splay;
                    k = search_and_splay(name, gBlock, num_comp2, num_splay2);
                    if (k != NULL) {
                        done = true;
                        if (k->type == gan->type) {
                            num_comp = num_comp2;
                            num_splay = num_splay2;
                            break;
                        }
                    }
                    gBlock--;
                }
                if (k == NULL || k->type != gan->type) {
                    if (done == false) throw Undeclared(s);
                    else if (done == true) throw TypeMismatch(s);
                }
                else k->value = gan->value;
            }
        }
        //XỬ LÝ GÁN HẰNG
        else {
            //GÁN HẰNG SỐ
            if (checkNumber(typ) == true) {
                int fBlock = this->cBlock;
                bool done = false;
                Symbol* ptr = NULL;
                while (fBlock >= 0) {
                    int num_comp2 = num_comp, num_splay2 = num_splay;
                    ptr = search_and_splay(name, fBlock, num_comp2, num_splay2);
                    if (ptr != NULL) {
                        done = true;
                        if (ptr->type == "number") {
                            num_comp = num_comp2;
                            num_splay = num_splay2;
                            break;
                        }
                    }
                    fBlock--;
                }
                if (ptr == NULL || ptr->type != "number") {
                    if (done == false) throw Undeclared(s);
                    else if (done == true) throw TypeMismatch(s);
                }
                else ptr->value = typ;
            }
            else if (checkString(typ) == true) {
                int fBlock = this->cBlock;
                bool done = false;
                Symbol* ptr = NULL;
                while (fBlock >= 0) {
                    int num_comp2 = num_comp, num_splay2 = num_splay;
                    ptr = search_and_splay(name, fBlock, num_comp2, num_splay2);
                    if (ptr != NULL) {
                        done = true;
                        if (ptr->type == "string") {
                            num_comp = num_comp2;
                            num_splay = num_splay2;
                            break;
                        }
                    }
                    fBlock--;
                }
                if (ptr == NULL || ptr->type != "string") {
                    if (done == false) throw Undeclared(s);
                    else if (done == true) throw TypeMismatch(s);
                }
                else ptr->value = typ;
            }
        }
        cout << num_comp << " " << num_splay;
    }
    //BEGIN
    void SymbolBegin(string s) {
        this->cBlock++;
    }
    //END
    void SymbolEnd(string s) {
        if (this->cBlock == 0)
            throw UnknownBlock();
        Node* run = this->lK->head;
        while (run) {
            Node* run2 = run;
            run = run->next;
            if (run2->block == this->cBlock) {
                string a = run2->val;
                lK->remove(run2);
                SymbolTable::remove(a, this->cBlock);
            }
        }
        this->cBlock--;
    }
    //LOOKUP
    void SymbolLookup(string s) {
        string code, name, typ, stat;
        divideString(s, code, name, typ, stat);
        int fBlock = this->cBlock;
        while (fBlock >= 0) {
            Symbol* sav = search_not_splay(name, fBlock);
            if (sav != NULL) {
                sav = search_and_splay(name, fBlock);
                break;
            }
            fBlock--;
        }
        if (fBlock < 0)
            throw Undeclared(s);
        cout << to_string(fBlock);
    }
    //PRINT
    void SymbolPrint(string s) {
        string ans = "";
        printPre(this->root, ans);
        if (ans != "") cout << ans << endl;
        else return;
    }
    void checkClose() {
        if (this->cBlock != 0)
            throw UnclosedBlock(this->cBlock);
    }

    //******************************************************
    //DESTRUCTOR********************************************
    //******************************************************
    ~SymbolTable() {
        this->lK->clear();
        delete this->lK;
        SymbolTable::clearK();
    }
};
#endif