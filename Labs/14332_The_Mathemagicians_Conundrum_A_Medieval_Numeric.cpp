#include <iostream>
#include <vector>
using namespace std;

//O(logn)
long long merge_and_count(vector<int>& arr, vector<int>& temp_arr, int left, int mid, int right) {
    int i = left;    // left subarray
    int j = mid + 1; // right subarray
    int k = left;    // index to be sorted
    long long inv_count = 0;

    // Conditions are checked to ensure that i doesn't exceed mid and j doesn't exceed right
    while ((i <= mid) && (j <= right)) {
        if (arr[i] <= arr[j]) {
            temp_arr[k++] = arr[i++];
        } else {
            // because all elements left to i in the left subarray
            // are greater than arr[j]
            temp_arr[k++] = arr[j++];
            inv_count += (mid - i + 1);
        }
    }

    // Copy the remaining elements of left subarray, if any
    while (i <= mid)
        temp_arr[k++] = arr[i++];

    // Copy the remaining elements of right subarray, if any
    while (j <= right)
        temp_arr[k++] = arr[j++];

    // Copy the sorted subarray into Original array
    for (i = left; i <= right; i++)
        arr[i] = temp_arr[i];

    return inv_count;
}

long long merge_sort_and_count(vector<int>& arr, vector<int>& temp_arr, int left, int right) {
    long long inv_count = 0;
    if (left < right) {
        int mid = (left + right) / 2;
        //using the other merge function
        inv_count += merge_sort_and_count(arr, temp_arr, left, mid);
        //cout<<"The count left to mid is:"<<inv_count;
        inv_count += merge_sort_and_count(arr, temp_arr, mid + 1, right);
        //cout<<"The count mid to right is:"<<inv_count;    
        //recursion
        inv_count += merge_and_count(arr, temp_arr, left, mid, right);
    }
    return inv_count;
}

int main() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }

    vector<int> temp_arr(n);
    //O(longn)
    long long result = merge_sort_and_count(p, temp_arr, 0, n - 1);

    cout << result << endl;
    return 0;
}