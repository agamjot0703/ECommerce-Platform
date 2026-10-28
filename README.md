# 🛒 E-Commerce Platform (C++ & Qt)

A modular, student-friendly desktop E-Commerce platform built with **C++17** and **Qt 6**, designed for university Object-Oriented Programming (OOP) laboratory evaluation.

---

## 🗂️ Project Directory Structure

```text
ECommercePlatform/
│
├── .gitignore                      # Git ignore rules for build artifacts & IDE files
├── ECommercePlatform.pro           # Qt project configuration file
├── README.md                       # Documentation & lab evaluation guide
│
├── data/                           # Flat-file database storage
│   ├── inventory.txt               # Product inventory catalogue
│   ├── users.txt                   # Registered user accounts & wallet balances
│   └── orders.txt                  # Completed customer order logs
│
└── src/                            # Modular application source code
    ├── main.cpp                    # Application entry point & initialization
    │
    ├── models/                     # Layer 1: OOP Entities & Business Logic
    │   ├── exceptions.h            # Custom exception classes (std::exception)
    │   ├── user.h / .cpp           # Abstract base class
    │   ├── customer.h / .cpp       # Customer class (wallet, cart, place order)
    │   ├── admin.h / .cpp          # Admin class (inventory management, sales)
    │   ├── product.h / .cpp        # Product entity with operator== overload
    │   ├── cart.h / .cpp           # Shopping cart with operator+= overload
    │   └── order.h / .cpp          # Order entity with record serialization
    │
    ├── storage/                    # Layer 2: Flat-File Persistence
    │   └── filemanager.h / .cpp    # Static utility class for .txt I/O parsing
    │
    └── views/                      # Layer 3: Qt Graphical User Interface
        ├── loginwindow.h / .cpp        # Login & user registration window
        ├── customerdashboard.h / .cpp  # Product store, search, wallet, cart
        ├── cartwindow.h / .cpp         # Cart modal, coupon codes, checkout
        └── admindashboard.h / .cpp     # Analytics bar, inventory editor, orders tab
```

---

## 📚 Key OOP Pillars & Concepts

| OOP Principle | Where It Is Used in Code |
| :--- | :--- |
| **Encapsulation** | Private variables across all core models (`User`, `Customer`, `Admin`, `Product`, `Cart`, `Order`) protected by public accessors and business logic methods. |
| **Inheritance** | Base abstract class `User` extended by derived classes `Customer` and `Admin`. |
| **Polymorphism** | Pure virtual methods in `User` (`displayRole()`, `toString()`) dynamically dispatched via base pointers (`User*`). |
| **Custom Exception Handling** | Custom exception hierarchy in `exceptions.h` inheriting from `std::exception` (`InsufficientStockException`, `InsufficientFundsException`, `InvalidCouponException`) handled via `try-catch` blocks. |
| **Operator Overloading** | Overloaded `Product::operator==` (compare product IDs) and `Cart::operator+=` (shorthand to add item pairs). |
| **Composition & Aggregation** | `Customer` owns a heap-allocated `Cart` (`cart = new Cart();`), while `Cart` aggregates `Product` objects. |
| **Flat-File Persistence** | Pure static utility class `FileManager` managing delimiter-separated (`|`) records without requiring SQL. |

---

## ✨ Features

- **Customer Store**:
  - Live search bar and category dropdown filter.
  - In-app customer wallet balance with deposit functionality (`➕ Add Funds`).
  - Discount promo codes (`SAVE10` for 10% off, `WELCOME20` for 20% off).
  - Visual low-stock badges (orange for $\le 3$ items, red for out-of-stock).
  - Purchase receipt text export (`Receipt_ORD-....txt`).
- **Admin Control Panel**:
  - Dynamic analytics summary bar (**Total Revenue**, **Orders Completed**, **Total Products / Low Stock**).
  - Product management form (Add, Update, Delete).
  - One-click **⚡ Restock (+10)** button.
  - Order history tab displaying all customer transactions.

---

## 🔑 Default Login Credentials

| Role | Username | Password | Starting Wallet |
| :--- | :--- | :--- | :--- |
| **Admin** | `admin` | `admin123` | N/A |
| **Customer** | `alice` | `alice123` | **$750.00** |

*(New accounts can also be created on the fly via the registration form).*

---

## 🚀 How to Open and Run

### Inside Qt Creator
1. Open **Qt Creator**.
2. Click **File** > **Open File or Project...** and select `ECommercePlatform.pro`.
3. Choose your desktop kit (Qt 6 with MinGW 64-bit) and click **Configure Project**.
4. Press the green **Run** button (or press `Ctrl+R`).

---

## 📤 How to Upload to GitHub

Follow these simple steps in terminal or PowerShell:

```powershell
# 1. Navigate to the project directory
cd "c:\Users\agamj\Desktop\p3"

# 2. Add all clean project files
git add .

# 3. Commit the files
git commit -m "Initial commit: E-Commerce Platform Qt C++ project"

# 4. Set the main branch
git branch -M main

# 5. Link to your GitHub repository (replace with your repo URL)
git remote add origin https://github.com/<your-username>/<your-repo-name>.git

# 6. Push the code
git push -u origin main
```
*(All temporary build artifacts, compiled binaries, and IDE caches are automatically excluded by `.gitignore`)*.
