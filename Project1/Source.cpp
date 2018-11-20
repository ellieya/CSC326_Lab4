#include <iostream>

using namespace std;

int main() {
	int x = 1;
	int try_pre = ++x;
	x = 1;
	int try_post = x++;

	cout << "x = " << x << "| PRE: " << try_pre << '\n'
		<< "x = " << x << "| POST: " << try_post << endl;

	system("pause");
	return 0;
}