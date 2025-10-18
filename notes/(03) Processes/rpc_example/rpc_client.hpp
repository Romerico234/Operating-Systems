#pragma once

// Forward declaration for uint16_t
typedef unsigned short uint16_t;

// RPC Client class to manage connection
class RpcClient {
public:
    int fd = -1;  // socket file descriptor
};

// Function declarations for RPC operations
bool rpc_connect(RpcClient& client, const char* ip, uint16_t port);
bool rpc_add(RpcClient& client, long long a, long long b, long long& result);
bool rpc_sub(RpcClient& client, long long a, long long b, long long& result);
bool rpc_mul(RpcClient& client, long long a, long long b, long long& result);
bool rpc_div(RpcClient& client, long long a, long long b, long long& result);
void rpc_close(RpcClient& client);
