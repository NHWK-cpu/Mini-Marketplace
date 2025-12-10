#include <iostream>
#include <string>
using namespace std;

struct Product {
    int id; string name;
    int price; int stock;
    Product* next;

    Product(int _id, string _name, int _price, int _stock) {
        id = _id; name = _name; price = _price;
        stock = _stock; next = NULL;
    }
};

class HashTable {
private:
    static const int TABLE_SIZE = 10;
    Product* table[TABLE_SIZE];

    int hashFunc(int id) {
        return id % TABLE_SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
    }

    void insertProduct(int id, string name, int price, int stock) {
        if (searchProduct(id) != NULL) {
            cout << "Gagal menambahkan produk! ID " << id << " sudah digunakan.\n";
            return;
        }

        int index = hashFunc(id);
        Product* newProd = new Product(id, name, price, stock);

        if (table[index] == NULL) {
            table[index] = newProd;
        }
        else {
            Product* temp = table[index];
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newProd;
        }

        cout << "Produk berhasil ditambahkan!\n";
    }

    Product* searchProduct(int id) {
        int index = hashFunc(id);
        Product* temp = table[index];

        while (temp != NULL) {
            if (temp->id == id) return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void displayProducts() {
        cout << "\n=== Daftar Produk ===\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Product* temp = table[i];
            while (temp != NULL) {
                cout << "ID: " << temp->id
                    << " | Nama: " << temp->name
                    << " | Harga: " << temp->price
                    << " | Stok: " << temp->stock << "\n";
                temp = temp->next;
            }
        }
    }
};

struct Order {
    int productId;
    int qty; Order* next;

    Order(int _id, int _qty) {
        productId = _id;
        qty = _qty; next = NULL;
    }
};

class OrderQueue {
private:
    Order* front; Order* rear;

public:
    OrderQueue() {
        front = rear = NULL;
    }

    void enqueue(int productId, int qty) {
        Order* newOrder = new Order(productId, qty);

        if (rear == NULL) {
            front = rear = newOrder;
        }
        else {
            rear->next = newOrder;
            rear = newOrder;
        }

        cout << "Pesanan masuk ke antrian!\n";
    }

    void processOneOrder(HashTable& ht) {
        if (front == NULL) {
            cout << "Tidak ada pesanan.\n";
            return;
        }

        Order* temp = front;
        Product* p = ht.searchProduct(temp->productId);

        if (p == NULL) {
            cout << "Produk tidak ditemukan!\n";
        }
        else if (p->stock < temp->qty) {
            cout << "Stok tidak cukup untuk produk: " << p->name << "\n";
        }
        else {
            p->stock -= temp->qty;
            cout << "Pesanan diproses: " << p->name << " (" << temp->qty << ")\n";
        }

        front = front->next;
        delete temp;

        if (front == NULL)
            rear = NULL;
    }

    void displayQueue() {
        cout << "\n=== Antrian Pesanan ===\n";
        Order* temp = front;

        if (temp == NULL) {
            cout << "Antrian kosong.\n";
            return;
        }

        while (temp != NULL) {
            cout << "Produk ID: " << temp->productId << " | Qty: " << temp->qty << "\n";
            temp = temp->next;
        }
    }
};

struct History {
    string desc;
    History* next;

    History(string _d) {
        desc = _d;
        next = NULL;
    }
};

class HistoryStack {
private:
    History* top;

public:
    HistoryStack() {
        top = NULL;
    }

    void push(string desc) {
        History* newH = new History(desc);
        newH->next = top; top = newH;
    }

    void pop() {
        if (top == NULL) return;
        History* temp = top; top = top->next; delete temp;
    }

    void showHistory() {
        cout << "\n=== Riwayat ===\n";
        History* temp = top;

        if (temp == NULL) {
            cout << "Riwayat kosong.\n";
            return;
        }

        while (temp != NULL) {
            cout << "- " << temp->desc << "\n";
            temp = temp->next;
        }
    }
};

int main() {
    HashTable products;
    OrderQueue queue;
    HistoryStack history;

    int choice, id, price, stock, qty;
    string name;

    while (true) {
        cout << "\n=== MINI MARKETPLACE ===\n";
        cout << "1. Tambah Produk (Owner)\n";
        cout << "2. Lihat Produk\n";
        cout << "3. Beli Produk (Buyer)\n";
        cout << "4. Lihat Antrian\n";
        cout << "5. Proses Pesanan (Owner)\n";
        cout << "6. Lihat Riwayat\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1:
            cout << "ID Produk: "; cin >> id;
            cout << "Nama: "; cin >> name;
            cout << "Harga: "; cin >> price;
            cout << "Stok: "; cin >> stock;
            if (id < 0 || price < 0 || stock < 0) {
                cout << "ID, harga, dan stok harus bernilai positif!\n";
                break;
			}

            products.insertProduct(id, name, price, stock);
            history.push("Tambah produk: " + name);
            break;

        case 2:
            products.displayProducts();
            break;

        case 3:
            cout << "Masukkan ID produk yang dibeli: "; cin >> id;
            cout << "Jumlah: "; cin >> qty;
            if (qty <= 0) {
                cout << "Jumlah harus lebih dari 0!\n";
                break;
            }
            else if (products.searchProduct(id) == NULL) {
                cout << "Produk dengan ID " << id << " tidak ditemukan!\n";
                break;
            }
            queue.enqueue(id, qty);
            history.push("Pesanan produk ID: " + to_string(id));
            break;

        case 4:
            queue.displayQueue();
            break;

        case 5:
            queue.processOneOrder(products);
            history.push("Owner memproses pesanan");
            break;
            
        case 6:
            history.showHistory();
            break;

        default:
            cout << "Menu tidak valid!\n";
        }
    }

    return 0;
}
