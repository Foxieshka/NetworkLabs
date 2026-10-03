#include <cstdio>
#include <cstdlib> // EXIT_ACCESS, EXIT_FAILURE
#include <cstring> // memset
#include <unistd.h> // close
#include <errno.h>
#include <sys/socket.h> // socket, bind
#include <arpa/inet.h>
#include <netinet/in.h>

// Объявляем константы

#define SERVER_IP "127.0.0.1" // IP адрес для привязки
#define SERVER_PORT 4444

int main(void){
    // Создание сокета
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        perror("socket");
        return EXIT_FAILURE;
    }
    // показываем какой номер получил сокет
    printf("Сокет создан успешно. Дескриптор: %d\n", sockfd);
    
    // Объявляем структуру адреса и обнуляем ее 
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    // Заполняем поля структуры
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Перевод адреса из текста в бинарный
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0){
        perror("inet_pton");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("Адрес заполнен: %s:%d\n", SERVER_IP, SERVER_PORT);

    // Проверяем - выводим адрес обратно.
    char ip_str[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &server_addr.sin_addr, ip_str,
    sizeof(ip_str)) == NULL){
        perror("inet_ntop");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("Проверка адреса: %s:%d\n", ip_str, ntohs(server_addr.sin_port));

    // Привязываем сокет
    if(bind(sockfd, (struct sockaddr *)&server_addr, 
    sizeof(server_addr)) < 0){
        perror("bind");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("Сокет привязан к адресу %s:%d\n", SERVER_IP, SERVER_PORT);
    // Вывод списка fd
    printf("--- Открытые FD до close() ---\n");
    system("ls -la /proc/$(pgrep -n lab1_middle)/fd/");
    // Закрываем сокет
    if(close(sockfd) < 0){
        perror("close");
        return EXIT_FAILURE;
    }
    printf("Сокет закрыт. Дескриптор %d освобожден.", sockfd);
    return EXIT_SUCCESS; // все завершилось успешно (0)
}