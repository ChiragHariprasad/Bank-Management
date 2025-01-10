# Bank Server Application

This project implements a simple server application in C that simulates a banking system. The server handles client requests for login, balance inquiry, and money transfer through a TCP connection.

## Features

1. **User Authentication**:
   - Validates user credentials from a file (`username.txt`).

2. **Balance Inquiry**:
   - Fetches the total balance and transaction details for a given user from a file (`mon.txt`).

3. **Money Transfer**:
   - Facilitates transferring money between users and updates the transaction records in `mon.txt`.

## Files Used

1. `username.txt`:
   - Stores user information such as username, password, and personal details.
   - Structure: `struct pass`.

2. `mon.txt`:
   - Records money transfer transactions.
   - Structure: `struct money`.

## Prerequisites

- **Windows Operating System** (uses Winsock API).
- Microsoft Visual Studio or any compatible C compiler.
- `Ws2_32.lib` for Winsock functionality.

## Compilation

Ensure the `Ws2_32.lib` library is linked during compilation. If using GCC, this is automatically handled by the `#pragma comment(lib, "ws2_32.lib")` directive.

## Usage

1. **Start the Server**:
   - Compile and run the server application.
   - The server listens on port `8080`.

2. **Client Requests**:
   - Send JSON-formatted requests to the server using a TCP client (e.g., Postman, netcat, or a custom client).

### Example Requests

#### Login
```json
{
  "action": "login",
  "username": "user1",
  "password": "password1"
}
```

#### Check Balance
```json
{
  "action": "check_balance",
  "username": "user1"
}
```

#### Transfer Money
```json
{
  "action": "transfer",
  "from_user": "user1",
  "to_user": "user2",
  "amount": 5000
}
```

### Example Responses

#### Login
```json
{"success": true}
```

#### Check Balance
```json
{
  "transactions": [
    {"from": "user2", "amount": 1000},
    {"from": "user3", "amount": 2000}
  ],
  "total_balance": 3000
}
```

#### Transfer Money
```json
{"success": true}
```

## Debugging

To debug server responses, the application logs outgoing responses in the console. These logs can help verify the JSON being sent to clients.

Example:
```
Sending response: {"success": true}
```

## Error Handling

- If files (`username.txt` or `mon.txt`) are missing, the server gracefully handles errors by returning appropriate JSON responses:

  ```json
  {"error": "Error opening file"}
  ```

- Invalid usernames or transactions are flagged with a failure response:

  ```json
  {"success": false}
  ```

## Limitations

1. No encryption for sensitive data (e.g., passwords).
2. Simple JSON handling using `sscanf` and string functions instead of a robust JSON parser.
3. Designed for demonstration purposes; lacks advanced error recovery.

## Future Improvements

- Integrate a proper JSON library (e.g., cJSON) for improved request/response handling.
- Add secure password storage (e.g., hashing).
- Implement multi-threading for handling multiple client connections.
- Extend functionality with more banking features.

## Author

This project is a simplified demonstration of a bank server application written in C using the Winsock API for networking. It is intended for educational purposes.
