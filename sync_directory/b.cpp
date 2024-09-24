#include<bits/stdc++.h>
using namespace std;
#define ll long long
ll rec(ll i,ll mnth,ll msal,ll preamt,vector<ll>&dp,vector<ll>&cost,vector<ll>&happ){
    if(i>=mnth)
        return 0;
    if(i<0)
        return 0;
    if(dp[i]!=-1)
        return dp[i];
    if(preamt>=cost[i]){
        ll buy = happ[i] + rec(i + 1, mnth, msal, preamt - cost[i] + msal, dp, cost, happ);
        ll notbuy = rec(i + 1, mnth, msal, preamt + msal, dp, cost, happ);
        return dp[i]=max(buy, notbuy);
    }
    return dp[i]=rec(i + 1, mnth, msal, preamt + msal, dp, cost, happ);
}
int main(){
    ll t;
    cin >> t;
    while(t--){
        ll mnth, msal;
        cin >> mnth >> msal;
        vector<ll> cost(mnth);
        vector<ll> happ(mnth);
        for (ll i = 0; i < mnth;i++){
            cin >> cost[i] >> happ[i];
        }
        vector<ll> dp(mnth , -1);
        ll hpns = 0;
        cout<<rec(0, mnth, msal,0, dp, cost, happ)<<endl;
    }
    return 0;
}