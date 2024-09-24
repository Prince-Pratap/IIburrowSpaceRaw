#include <bits/stdc++.h>
using namespace std;
#define ll long long
bool cows(vector<ll> &stalls, int minDis, int n, int C)
{
    int ans = 1, start = stalls[0];
    for (int i = 1; i < n; i++)
    {
        if (minDis <= stalls[i] - start)
        {
            ans++;
            start = stalls[i];
        }
        if (ans >= C)
            return true;
    }
    return false;
}
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int N, C;
        cin >> N >> C;
        vector<ll> stalls(N);
        for (int i = 0; i < N; i++)
        {
            cin >> stalls[i];
        }
        sort(stalls.begin(), stalls.end());
        int l = 1, r = stalls[N - 1] - stalls[0];
        while (l <= r)
        {
            int mid = (l + r) / 2;
            bool cow = cows(stalls, mid, N, C);
            if (cow)
                l = mid + 1;
            else
                r = mid - 1;
        }
        cout << r;
    }
    return 0;
}
