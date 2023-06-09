#include "OnlineStore.h"
#include <iostream>
#include <fstream>
#include "Admin.h"

using namespace std;

// Blank the singleton store instance
OnlineStore* OnlineStore::instance = 0;

OnlineStore::OnlineStore() {
	curUser = 0;
}

void OnlineStore::initialize() {
	fstream fp;
	fp.open("db.txt", ios::in);
	if (!fp) {
		// File not exist
		// Add new Admin with email: admin@gmail.com password: 1234
		users.push_back(new Admin());
	}
	else {
		// Load from File
		while (!fp.eof()) {
			string type_token;
			fp >> type_token;
			cout << type_token << " ";
			if (type_token == "USER") {
				string user_type_token;
				string _email, _password, _name, _address, _number;
				fp >> user_type_token;
				cout << user_type_token << " ";
				if (user_type_token == "0") { // Admin
					fp >> _email >> _password;
					cout << _email << " " << _password;
					users.push_back(new Admin(_email, _password));
				}
				else if (user_type_token == "1") { // Manager
					fp >> _email >> _password >> _name;
					cout << _email << " " << _password << " " << _name;
					users.push_back(new Manager(_email, _password, _name));
				}
				else if (user_type_token == "2") { // Customer
					fp >> _email >> _password >> _name >> _address >> _number;
					cout << _email << " " << _password << " " << _name << " " << _address << " " << _number;
					users.push_back(new Customer(_email, _password, _name, _address, _number));
				}
			}
			else if (type_token == "PRODUCT") {
				string _pin, _name;
				double _rate;
				int _amount;
				fp >> _pin >> _name >> _rate >> _amount;
				cout << _pin << " " << _name << " " << _rate << " " << _amount;
				products.push_back(new Product(_pin, _name, _rate, _amount));
			}
			else if (type_token == "ORDER") {
				string _oin, _customer_email;
				double _price;
				int _count;
				fp >> _oin >> _customer_email >> _price >> _count;
				cout << _oin << " " << _customer_email << " " << _price << " " << _count;
				orders.push_back(new Order(_oin, _customer_email, _price, _count));
			}
			else if (type_token == "ORDERITEM") {
				string _oin, _pin, _pname;
				double _price;
				int _count;
				fp >> _oin >> _pin >> _pname >> _price >> _count;
				cout << _oin << " " << _pin << " " << _pname << " " << _price << " " << _count;
				orderItems.push_back(new OrderItem(_oin, _pin, _pname, _price, _count));
			}
			cout << endl;
		}
		fp.close();
	}
}


void OnlineStore::mainMenu() {
	string email, password, choice;
	while (true) {
		system("CLS");
		cout << "Welcome to Main Menu" << endl;
		cout << "Enter your choice (1-Login,2-Exit): ";
		cin >> choice;
		while (choice == "1") {
			cout << endl << " ========== LOGIN ========== " << endl << endl;
			cout << "Enter Email: ";
			cin >> email;
			cout << "Enter Password: ";
			cin >> password;

			curUser = authenticate(email, password);
			if (curUser) {
				curUser->userMenu();
				// when user exits userMenu. then redirects choice
				break;
			}
			else {
				cout << endl << "Login Failed: Wrong Email or Password" << endl;
			}
		}
		if (choice == "2") {
			// Exit app
			exit(1);
		}
	}
}

User* OnlineStore::authenticate(string _email, string _password) {
	for (User* user : users) {
		if (user->getEmail() == _email && user->getPassword() == _password) {
			return user;
		}
	}
	return NULL;
}

bool OnlineStore::addNewManager(string _email, string _password, string _name) {
	for (User* user : users) {
		if (*user == _email) {
			return false;
		}
	}
	users.push_back(new Manager(_email, _password, _name));
	saveToFile();
	return true;
}

bool OnlineStore::deleteManager(string _email) {
	for (User* user : users) {
		if (user->getType() == USER_TYPE_MANAGER && *user == _email) {
			users.remove(user);
			saveToFile();
			return true;
		}
	}
	return false;
}

bool OnlineStore::updateManager(string _email, string _password, string _name) {
	for (User* user : users) {
		if (user->getType() == USER_TYPE_MANAGER && *user == _email) {
			Manager* m_user = (Manager*)user;
			user->setPassword(_password);
			m_user->setName(_name);
			saveToFile();
			return true;
		}
	}
	return false;
}

void OnlineStore::showManagers() {
	int count = 0;
	printf(" === MANAGER LIST === \n");
	printf("%16s %16s %16s\n", "Email", "Name", "Password");
	for (User* user : users) {
		if (user->getType() == USER_TYPE_MANAGER) {
			Manager* m_user = (Manager *)user;
			printf("%16s %16s %16s\n", user->getEmail().c_str(), m_user->getName().c_str(), user->getPassword().c_str());
			count += 1;
		}
	}
	printf("Total: %d Managers Registered\n\n", count);
}

Manager* OnlineStore::searchManager(string _email) {
	for (User* user : users) {
		if (*user == _email) {
			return (Manager*)user;
		}
	}
	return NULL;
}


bool OnlineStore::addNewCustomer(string _email, string _password, string _name, string _address, string _number) {
	for (User* user : users) {
		if (*user == _email) {
			return false;
		}
	}
	users.push_back(new Customer(_email, _password, _name, _address, _number));
	saveToFile();
	return true;
}

bool OnlineStore::deleteCustomer(string _email) {
	for (User* user : users) {
		if (user->getType() == USER_TYPE_CUSTOMER && *user == _email) {
			users.remove(user);
			saveToFile();
			return true;
		}
	}
	return false;
}

bool OnlineStore::updateCustomer(string _email, string _password, string _name, string _address, string _number) {
	for (User* user : users) {
		if (user->getType() == USER_TYPE_CUSTOMER && *user == _email) {
			Customer* m_user = (Customer*)user;
			user->setPassword(_password);
			m_user->setName(_name);
			m_user->setAddress(_address);
			m_user->setNumber(_number);
			saveToFile();
			return true;
		}
	}
	return false;
}

void OnlineStore::showCustomers() {
	int count = 0;
	printf(" === CUSTOMER LIST === \n");
	printf("%16s %16s %16s %16s %16s\n", "Email", "Name", "Password", "Address", "Number");
	for (User* user : users) {
		if (user->getType() == USER_TYPE_CUSTOMER) {
			Customer* m_user = (Customer*)user;
			printf("%16s %16s %16s %16s %16s\n", user->getEmail().c_str(), m_user->getName().c_str(), user->getPassword().c_str(), m_user->getAddress().c_str(),
				m_user->getNumber().c_str());
			count += 1;
		}
	}
	printf("Total: %d Customers Registered\n\n", count);
}

Customer* OnlineStore::searchCustomer(string _email) {
	for (User* user : users) {
		if (*user == _email) {
			return (Customer*)user;
		}
	}
	return NULL;
}


bool OnlineStore::addNewProduct(string _pin, string _name, double _rate, int _amount) {
	for (Product* product : products) {
		if (*product == _pin) {
			return false;
		}
	}
	products.push_back(new Product(_pin, _name, _rate, _amount));
	saveToFile();
	return true;
}

bool OnlineStore::deleteProduct(string _pin) {
	for (Product* product : products) {
		if (*product == _pin) {
			products.remove(product);
			saveToFile();
			return true;
		}
	}
	return false;
}

bool OnlineStore::updateProduct(string _pin, string _name, double _rate, int _amount) {
	for (Product* product : products) {
		if (*product == _pin) {
			product->setName(_name);
			product->setRate(_rate);
			product->setAmount(_amount);
			saveToFile();
			return true;
		}
	}
	return false;
}

void OnlineStore::showProducts() {
	int count = 0;
	printf(" === PRODUCT LIST === \n");
	printf("%16s %16s %16s %16s\n", "Identification", "Name", "Rate", "Amount");
	for (Product* product : products) {
		printf("%16s %16s %16lf %16d\n", product->getPin().c_str(), product->getName().c_str(), product->getRate(), product->getAmount());
		count += 1;
	}
	printf("Total: %d Products Registered\n\n", count);
}

Product* OnlineStore::searchProduct(string _pin) {
	for (Product* product : products) {
		if (*product == _pin) {
			return product;
		}
	}
	return NULL;
}

void OnlineStore::addNewOrder(Order* _order, OrderItem* _orderItems[], int _itemCnt) {
	orders.push_back(_order);
	for (int i = 0; i < _itemCnt; i++) {
		orderItems.push_back(_orderItems[i]);
	}
	saveToFile();
}

Order* OnlineStore::searchOrder(string _oin) {
	for (Order* order : orders) {
		if (*order == _oin) {
			return order;
		}
	}
	return NULL;
}

void OnlineStore::showOrders() {
	printf("%16s %16s %16s %16s\n", "Order ID", "Customer Email", "Price", "Amount");
	for (Order* order : orders) {
		printf("%16s %16s %16lf %16d\n", order->getOin().c_str(), order->getCustomerEmail().c_str(), order->getPrice(), order->getCount());
	}
}

void OnlineStore::showOrder(string _oin) {
	Order* order = searchOrder(_oin);
	if (order == NULL) {
		cout << "Order Identification Number Not Exist!" << endl;
		return;
	}
	printf("%16s %16s %16s %16s\n", "Product ID", "Product Name", "Price", "Amount");
	for (OrderItem* orderItem : orderItems) {
		if (orderItem->getOin() == _oin) {
			printf("%16s %16s %16lf %16d\n", orderItem->getPin().c_str(), orderItem->getPname().c_str(), orderItem->getPrice(), orderItem->getCount());
		}
	}
	printf("Total Price: %lf\nTotal Amount: %d\n", order->getPrice(), order->getCount());
}

void OnlineStore::showCustomerOrders(string _customer_email) {
	for (Order* order : orders) {
		if (order->getCustomerEmail() == _customer_email) {
			printf(" ===== %s ===== \n", order->getOin().c_str());
			showOrder(order->getOin());
		}
	}
}

void OnlineStore::saveToFile() {
	fstream fp;
	fp.open("db.txt", ios::out);
	for (User* user : users) {
		fp << *user;
		// cout << *user;
	}
	for (Product* product : products) {
		fp << *product;
	}
	for (Order* order : orders) {
		fp << *order;
	}
	for (OrderItem* orderItem : orderItems) {
		fp << *orderItem;
	}
	fp.close();
}

OnlineStore* OnlineStore::getInstance() {
	if (instance == 0) {
		instance = new OnlineStore();
		// Initialize the store.
		instance->initialize();
	}
	return instance;
}