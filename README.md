# socket_tcp_class
使用方法：  
server端建立连接:  
socket_server test;  
thread_socket tem;  
test.setup_port_ip(8888,"127.0.0.1");  
tem.Th_getmessage_server(test);  
test.send_message();  
test.close_socket();  
  
client端建立连接:  
socket_client test;  
thread_socket tem;  
test.setup_port_ip(8888,"127.0.0.1");  
tem.Th_sendmessage_client(test);  
test.get_message();  
test.close_socket();  

thread_socket类用于创建多线程，类内的四个方法均以多线程的方式对消息进行接收与发送
