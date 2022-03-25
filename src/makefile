CC = gcc -std=gnu99

jarvis:jvs_thread.o jarvis.o jvs_string.o jvs_hash.o jvs_epoller.o jvs_priority_queue.o jvs_buffer_io.o jvs_http_connect.o jvs_timer.o jvs_http_request.o jvs_thread_pool.o
	$(CC) *.o -o jarvis -lpthread -I .

jarvis.o:jarvis.c jvs_core.h
	$(CC) -g -c jarvis.c -I .

jvs_epoller.o:jvs_epoller.c jvs_epoller.h
	$(CC) -g -c jvs_epoller.c -I .

jvs_priority_queue.o:jvs_priority_queue.c jvs_priority_queue.h
	$(CC) -g -c jvs_priority_queue.c -I .

jvs_thread.o:jvs_thread_id.c jvs_thread_mutex.c jvs_thread_cond.c jvs_thread.h
	$(CC) -g -c jvs_thread_id.c jvs_thread_mutex.c jvs_thread_cond.c -I .

jvs_timer.o:jvs_timer.c jvs_timer.h
	$(CC) -g -c jvs_timer.c -I .

jvs_string.o:jvs_string.c jvs_string.h
	$(CC) -g -c jvs_string.c -I .

jvs_hash.o:jvs_hash.c jvs_hash.h
	$(CC) -g -c jvs_hash.c -I .

jvs_buffer_io.o:jvs_buffer_io.c jvs_buffer_io.h
	$(CC) -g -c jvs_buffer_io.c -I .

jvs_http_connect.o:jvs_http_connect.c jvs_http_connect.h
	$(CC) -g -c jvs_http_connect.c -I .

jvs_http_request.o:jvs_http_request.c jvs_http_request.h
	$(CC) -g -c jvs_http_request.c -I .

jvs_thread_pool.o:jvs_thread_pool.c jvs_thread_pool.h
	$(CC) -g -c jvs_thread_pool.c -I .

clean:
	rm *.o jarvis
