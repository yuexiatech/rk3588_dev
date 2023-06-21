#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_SIZE 200

int main() {
    char command[COMMAND_SIZE];
    char server_ip[16];

    printf("请输入服务器的IP地址：");
    fgets(server_ip, sizeof(server_ip), stdin);
    server_ip[strcspn(server_ip, "\n")] = '\0';  // 移除换行符

    sprintf(command, "iperf -c %s -i 1 -t 10", server_ip);  // 使用iperf发送请求进行带宽测试

    printf("正在测试带宽，请稍候...\n");

    system(command);

    printf("带宽测试已完成。\n");

    return 0;
}