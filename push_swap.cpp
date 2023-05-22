#include <iostream>
#include <algorithm>
#include <deque>
#include <sstream>
#define A 0
#define B 1

using namespace std;
int arr[1000001];
int n;
deque<int> dq_a, dq_b;
deque<int> dqs[2] = {dq_a, dq_b};
stringstream ss;
int cnt = 0;

void RO(int stack_idx) {
	int move_num = dqs[stack_idx].front();
	dqs[stack_idx].pop_front();
	dqs[stack_idx].push_back(move_num);
	ss << "RO " << stack_idx << "0\n";
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

void sort_over_3(int from, int size) {
	if (from == A) {
		for (int i = size; i == 3; i--)
			PP(from, (from + 1) % 2);
	} else {

	}

}

void sort_less_3(int from, int size) {
	if (from == A) {
		if (size == 2)
			if (dqs[A][0] > dqs[A][1])
				RO(A);
		if (size == 3) {
			if (dqs[A][0] > dqs[])
		}
	}
	else {
		if (size == 2) {
			if (dqs[B][0] > dqs[B][1]) {
				PP(B, A);
				PP(B, A);
			} else {
				PP(B, A);
				RO(A);
				PP(B, A);
				RRO(A);
			}
		}
		if (size == 3) {

		}
	}
}

void partition(int from, int size, int ps[3]) {
	int dq_start_idx = dqs[from].size() - size;
	for (int i = 0; i < size; i++)
		arr[i] = dqs[from][dq_start_idx + i];
	sort(arr, arr + size);
	int pivot = arr[size / 3];
	int pivot2 = arr[(size / 3) * 2];
	ps[0] = size / 3; ps[1] = size / 3; ps[2] = size / 3 + size % 3;
	//A에서 파티션이 일어난 경우
	if (from == A) {
		for (int i = 0; i < size; i++) {
			//작은영역(S)
			if (dqs[from][dq_start_idx + i] < pivot) {
				PP(A, B);
			//중간영역(M)
			} else if (pivot <= dqs[from][dq_start_idx + i] && dqs[from][dq_start_idx + i] < pivot2) {
				PP(A, B);
				RO(B);
			//큰영역(L)
			} else if (dqs[from][dq_start_idx + i] >= pivot2) {
				RO(A);
			}
		}
		for (int j = 0; j < ps[2]; j++)
			RRO(A);
		for (int j = 0; j < ps[1]; j++)
			RRO(B);
	}
	//B에서 파티션이 일어난 경우
	else {
		for (int i = 0; i < size; i++) {
			if (dqs[from][dq_start_idx + i] < pivot) {
				RO(B);
			} else if (pivot <= dqs[from][dq_start_idx + i] && dqs[from][dq_start_idx + i] < pivot2) {
				PP(B, A);
				RO(A);
			} else if (dqs[from][dq_start_idx + i] >= pivot2) {
				PP(B, A);
			}
		}
		for (int j = 0; j < ps[0]; j++)
			RRO(B);
	}
}

//주어진 영역을 3등분하면서 정렬을 진행하는 함수
void order(int from, int size) {
	if (size <= 5) {
		if (size > 3)
			sort_over_3(from, size);
		sort_less_3(from, size);
		return ;
	}
	//S, M, L 영역 순으로 파티셔닝된 사이즈를 보관
	int partition_size[3] = {0, 0, 0};
	partition(from, size, partition_size);
	if (from == A) {
		order(A, partition_size[2]);
		order(B, partition_size[1]);
		order(B, partition_size[0]);
	}
	if (from == B) {
		order(A, partition_size[2]);
		for (int i = 0; i < partition_size[1]; i++)
			RRO(A);
		order(A, partition_size[1]);
		order(B, partition_size[0]);
	}

}

int main(void) {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
		dq_a.push_back(arr[i]);
	}
	order(A, n);
	cout << cnt << "\n" << ss.str() << endl;

}
