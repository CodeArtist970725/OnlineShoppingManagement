#include "Customer.h"
#include "OnlineStore.h"

Customer::Customer(string _email, string _password, string _name, string _address, string _number)
	: User(_email, _password, USER_TYPE_CUSTOMER) {
	name = _name;
	address = _address;
	number = _number;
}

void Customer::userMenu() {
	string choice;
	system("CLS");
	cout << " === Welcome to Customer Menu === " << endl;
	cout << "Current Customer Name: " << name << endl;
	while (true) {
		cout << "Enter your choice (1-Show Orders,2-Place a Order,3-Sign Out): ";
		cin >> choice;
		if (choice == "1") {
			showOrders();
		}
		if (choice == "2") {
			placeOrderMenu();
		}
		if (choice == "3") {
			return;
		}
	}
}

void Customer::showOrders() {
	parent->showCustomerOrders(email);
}

void Customer::placeOrderMenu() {
	string choice;
	string _pin;
	int _count;
	string _oin;

	system("CLS");
	cout << " === Welcome to Place a Order Menu for Customer === " << endl;

	while (true) {
		cout << "First of all, please input order identification number: ";
		cin >> _oin;

		if (parent->searchOrder(_oin) == NULL) {
			break;
		}
		else {
			cout << "Identification Number Exist! Please Enter Another" << endl;
		}
	}

	Order* order = new Order(_oin, email);
	OrderItem* orderItems[64];
	int itemCnt = 0;

	while (true) {
		cout << "Enter your choice (1-Show Cart,2-Add Product into the Cart,3-Place Order,4-Return to Customer Menu): ";
		cin >> choice;
		if (choice == "1") {
			// show cart
			printf("%16s %16s %16s\n", "Product Name", "Price", "Amount");
			for (int i = 0; i < itemCnt; i++) {
				printf("%16s %16lf %16d\n", orderItems[i]->getPname().c_str(), orderItems[i]->getPrice(), orderItems[i]->getCount());
			}
			printf("Total Price: %lf\nTotal Amount: %d\n", order->getPrice(), order->getCount());
		}
		if (choice == "2") {
			// add product into the cart
			parent->showProducts();
			cout << "Enter Product Identification Number: ";
			cin >> _pin;
			cout << "Enter Amount you are going to buy: ";
			cin >> _count;

			Product* product = parent->searchProduct(_pin);
			if (product) {
				int existFlag = 0;
				for (int i = 0; i < itemCnt; i++) {
					if (orderItems[i]->getPin() == _pin) {
						existFlag = 1;
						if (orderItems[i]->getCount() + _count <= product->getAmount()) {
							orderItems[i]->setCount(orderItems[i]->getCount() + _count);
							order->setCount(order->getCount() + _count);
							order->setPrice(order->getPrice() + product->getRate() * _count);
						}
						else {
							cout << "The amount is too much." << endl;
						}
						break;
					}
				}
				if (existFlag == 0) {
					if (_count <= product->getAmount()) {
						orderItems[itemCnt] = new OrderItem(_oin, _pin, product->getName(), product->getRate(), _count);
						order->setCount(order->getCount() + _count);
						order->setPrice(order->getPrice() + product->getRate() * _count);
						itemCnt++;
					}
					else {
						cout << "The amount is too much." << endl;
					}
				}
			}
			else {
				cout << "Identification Number Not Exist!" << endl;
			}
		}
		if (choice == "3") {
			// place order
			if (itemCnt > 0) {
				parent->addNewOrder(order, orderItems, itemCnt);
				for (int i = 0; i < itemCnt; i++) {
					Product* product = parent->searchProduct(orderItems[i]->getPin());
					product->setAmount(product->getAmount() - orderItems[i]->getCount());
				}
				cout << "Order has been placed successfully." << endl;
				parent->saveToFile();
				return;
			}
			else {
				cout << "Can not place blank order." << endl;
			}
		}
		if (choice == "4") {
			return;
		}
	}
}

void Customer::print(ostream& out) {
	out << name << " " << address << " " << number;
}

string Customer::getName() { return name; }

void Customer::setName(string _name) { name = _name; }