#ifndef INC_TASKWEBSOCKET_H_
#define INC_TASKWEBSOCKET_H_

#include "globals.h"

// Server HTTP + WebSocket
extern AsyncWebServer server;
extern AsyncWebSocket   ws;

// Khởi tạo WebSocket endpoint "/ws" và bắt đầu server (gọi 1 lần trong setup)
void initWebSocket();

// Gửi chuỗi JSON đến tất cả client đang kết nối
void broadcastWS(const String &msg);

#endif /* INC_TASKWEBSOCKET_H_ */
