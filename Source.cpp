#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

void f1();
void f2();

LONG a[10];
HANDLE hSemaphore;

int main() {
	for (int i = 0; i < 10; i++) a[i] = 0;
	hSemaphore = CreateSemaphore(NULL, 0, 21, "MySemaphore");
	if (hSemaphore == NULL) 
		cout << "Create semaphore failed" << GetLastError() << endl;
	HANDLE thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f1, NULL, 0, 0);
	if (thread1 == NULL) return GetLastError();
	HANDLE thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)f2, NULL, 0, 0);
	if (thread2 == NULL) return GetLastError();
	cin.get();
	CloseHandle(hSemaphore);
	TerminateThread(thread1, 0);
	CloseHandle(thread1);
	TerminateThread(thread2, 0);
	CloseHandle(thread2);
	return 0;
}

void f1() {
	srand(time_t(NULL));
	while (true)
	{
		for (int i = 0; i < 10; i++) {
			a[i] = rand() * 500 / (RAND_MAX);
			Sleep(220);
		}
		ReleaseSemaphore(hSemaphore, 1, NULL);
	}
	return;
}

void f2() {
	srand(time_t(NULL));
	while (true)
	{
		WaitForSingleObject(hSemaphore, INFINITE);
		cout << "Processed array: ";
		for (int i = 0; i < 9; i++) {
			if (abs(a[i]) % 2 == 1) a[i] = 1000;
			cout << a[i] << " ";
			Sleep(100);
		}
		cout << endl;
	}
	return;
}