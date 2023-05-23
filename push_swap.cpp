#include <iostream>
#include <algorithm>
#include <deque>
#include <sstream>
#include <fstream>
#define A 0
#define B 1

using namespace std;
int arr[100001];
int n;
deque<int> dqs[2];
stringstream ss;
int cnt = 0;


void RO(int stack_idx) {
	int move_num = dqs[stack_idx].front();
	dqs[stack_idx].pop_front();
	dqs[stack_idx].push_back(move_num);
	ss << "RO " << stack_idx << "\n";
	cnt++;
}

void PP(int from_idx, int to_idx) {
	int move_num = dqs[from_idx].front();
	dqs[from_idx].pop_front();
	dqs[to_idx].push_front(move_num);
	ss << "PP " << from_idx << " " << to_idx << "\n";
	cnt++;
}

void RRO(int stack_idx) {
	int move_num = dqs[stack_idx].back();
	dqs[stack_idx].pop_back();
	dqs[stack_idx].push_front(move_num);
	ss << "RRO " << stack_idx << "\n";
	cnt++;
}

void sort_less_3(int from, int size, char type) {
	if (from == A) {
		if (size == 2)
			if (dqs[A][0] > dqs[A][1]) {
				PP(A, B);RO(A);PP(B, A);RRO(A);
			}
		if (size == 3) {
			if (dqs[A][0] > dqs[A][1] && dqs[A][1] > dqs[A][2]) { // case 3 2 1
				RO(A);PP(A, B);PP(A, B);RRO(A);RO(B);PP(B, A);RRO(B);PP(B, A);
			} else if (dqs[A][0] > dqs[A][1] && dqs[A][1] < dqs[A][2] && dqs[A][0] > dqs[A][2]) { // case 3 1 2
				RO(A);PP(A, B);PP(A, B);RRO(A);PP(B, A);PP(B, A);
			} else if (dqs[A][0] > dqs[A][1] && dqs[A][1] < dqs[A][2] && dqs[A][0] < dqs[A][2]) { // case 2 1 3
				RO(A);PP(A,B),RRO(A),PP(B,A);
			} else if (dqs[A][0] < dqs[A][1] && dqs[A][0] > dqs[A][2]) { // case 2 3 1
				RO(A);RO(A);PP(A, B);RRO(A);RRO(A);PP(B, A);
			} else if (dqs[A][0] < dqs[A][1] && dqs[A][0] < dqs[A][2] && dqs[A][1] > dqs[A][2]) { // case 1 3 2
				PP(A, B);RO(A);PP(A, B);RRO(A);PP(B, A);PP(B, A);
			}
		}
	}
	else if (from == B) {
		if (size == 2) {
			if (dqs[B][0] > dqs[B][1]) {
				PP(B, A);PP(B, A);
			} else {
				PP(B, A);RO(A);PP(B, A);RRO(A);
			}
		}
		if (size == 3) {
			if (dqs[B][0] > dqs[B][1] && dqs[B][1] > dqs[B][2]) { // case 3 2 1
				PP(B, A);PP(B, A);PP(B, A);
			} else if (dqs[B][0] > dqs[B][1] && dqs[B][1] < dqs[B][2] && dqs[B][0] > dqs[B][2]) { // case 3 1 2
				PP(B, A);RO(B);PP(B, A);RRO(B);PP(B, A);
			} else if (dqs[B][0] > dqs[B][1] && dqs[B][1] < dqs[B][2] && dqs[B][0] < dqs[B][2]) { // case 2 1 3
				PP(B, A);RO(A);RO(B);PP(B, A);RRO(A);RRO(B);PP(B, A);
			} else if (dqs[B][0] < dqs[B][1] && dqs[B][0] > dqs[B][2]) { // case 2 3 1
				PP(B, A);RO(A);PP(B, A);RRO(A);PP(B, A);
			} else if (dqs[B][0] < dqs[B][1] && dqs[B][0] < dqs[B][2] && dqs[B][1] > dqs[B][2]) { // case 1 3 2
				PP(B, A);RO(A);PP(B, A);PP(B, A);RRO(A);
			} else if (dqs[B][0] < dqs[B][1] && dqs[B][0] < dqs[B][2] && dqs[B][1] < dqs[B][2]) { // case 1 2 3
				PP(B, A);RO(A);RO(B);PP(B, A);RRO(B);PP(B, A);RRO(A);
			}
		}
	}
}

void partition(int from, int size, int *ps, char type) {
	for (int i = 0; i < size; i++)
		arr[i] = dqs[from][i];
	sort(&arr[0], arr + size);
	int pivot = arr[size / 3];
	int pivot2 = arr[(size / 3) * 2];
	ps[0] = size / 3; ps[1] = size / 3; ps[2] = size / 3 + size % 3;
	deque<int>::iterator iter;
	int remain_s = ps[0];
	int remain_m = ps[1];
	int remain_b = ps[2];
	//A에서 파티션이 일어난 경우
	if (from == A) {
		for (int i = 0; i < size; i++) {
			//작은영역(S)
			if (dqs[A].front() < pivot) {
				PP(A, B);
				RO(B);
				remain_s--;
				//중간영역(M)
			} else if (pivot <= dqs[A].front() && dqs[A].front() < pivot2) {
				PP(A, B);
				remain_m--;
				//큰영역(L)
			} else if (dqs[A].front() >= pivot2 && remain_s != 0 && remain_m != 0) {
				RO(A);
				remain_b--;
			}
		}
		//RRO를 해서 파티셔닝한 L을 위로 올리는게 나은지, RO를 해서 고정영역을 밑으로 내리는게 나은지 확인(최적화 과정)
		if (2 * (ps[2] - remain_b) < dqs[A].size())
			for (int j = 0; j < ps[2] - remain_b; j++)
				RRO(A);
		else
			for (int j = 0; j < dqs[A].size() - ps[2] + remain_b; j++)
				RO(A);
	}
		//B에서 파티션이 일어난 경우
	else {
		for (int i = 0; i < size; i++) {
			if (dqs[B].front() < pivot && remain_m != 0 && remain_b != 0) {
				RO(B);
				remain_s--;
			} else if (pivot <= dqs[B].front() && dqs[B].front() < pivot2) {
				PP(B, A);
				RO(A);
				remain_m--;
			} else if (dqs[B].front() >= pivot2) {
				PP(B, A);
				remain_b--;
			}
		}
		if (2 * (ps[0] - remain_s) < dqs[B].size())
			for (int j = 0; j < ps[0] - remain_s; j++)
				RRO(B);
		else
			for (int j = 0; j < dqs[B].size() - ps[2] + remain_s; j++)
				RO(B);
	}
}

//주어진 영역을 3등분하면서 정렬을 진행하는 함수
void order(int from, int size, char type) {
	if (size == 1) {
		if (from == B)
			PP(B, A);
		return ;
	}
	if (size <= 3) {
		sort_less_3(from, size, type);
		return ;
	}
	//S, M, L 영역 순으로 파티셔닝된 사이즈를 보관
	int partition_size[3] = {0, 0, 0};
	partition(from, size, partition_size, type);
	if (from == A) {
		order(A, partition_size[2], 'L'); //L영역 정렬
		order(B, partition_size[1], 'M'); //M영역 정렬
		for (int i = 0; i < partition_size[0]; i++)
			RRO(B);
		order(B, partition_size[0], 'S'); //S영역 정렬
	}
	if (from == B) {
		order(A, partition_size[2], 'L');
		for (int i = 0; i < partition_size[1]; i++)
			RRO(A);
		order(A, partition_size[1], 'M');
		order(B, partition_size[0], 'S');
	}

}

int main(void) {

	/*
	ifstream in("a.txt");
	n = 100000;
	for (int i = 0; i < n; i++) {
		int temp;
		in >> temp;
		dqs[A].push_back(temp);
	}
	*/
	cin >> n;
	for (int i = 0; i < n; i++) {
		int temp;
		cin >> temp;
		dqs[A].push_back(temp);
	}
	order(A, n, 'L');
	cout << cnt << "\n" << ss.str();
	deque<int>::iterator iter;
	int n = 0;
	for (iter = dqs[A].begin(); iter != dqs[A].end(); iter++) {
		cout << *iter << " ";
		if (n == 5000) {
			cout << endl;
			n = 0;
		}
		n++;
	}
	cout << cnt << endl;
}
