#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void usage (char *program)
{
    printf ("Usage ./%s ip destino/gateway/máscara/interface", program);
    exit (1);
}


int main(int argc, char** argv) {

	if(argc < 3)
        usage(argv[0]);

    bool defaultRoute = false;
    bool notDefaultRoute = false;
    int indexDefaultRoute = -1;
    int numberMatch = 0;
    int longestMatch = 0;
    int indexEntradaLongestMatch = -1;
    int ponteiroFimIP = -1;
    char *gateway = malloc(15 * sizeof(1));
    char *mascara = malloc(15 * sizeof(1));
    char *interface = malloc(15 * sizeof(1));
    bool stopLongestMatch = false;

    char *ipOrigem = argv[1];

    char ipDestino[strlen(ipOrigem)];

    int numeroDeEntradas = argc - 2;

    for(int entradaAtual = 0; entradaAtual < numeroDeEntradas; entradaAtual++){

    	char *entrada = argv[2 + entradaAtual];

    	char possivelIpDestino[strlen(ipOrigem)];

    	int ponteiro = 0;
    	while(entrada[ponteiro] != '/'){
    		if(entrada[ponteiro] == ipOrigem[ponteiro] && stopLongestMatch == false){
    			if(entrada[ponteiro] != '.')
    				numberMatch++;
                if(entrada[ponteiro]!=0){
                    notDefaultRoute = true;
                }
    			possivelIpDestino[ponteiro] = entrada[ponteiro];
    			//printf("%c %c\n", entrada[ponteiro], ipOrigem[ponteiro]);
    			ponteiro++;
    			//printf("entrei\n");
    		}
    		else if((entrada[ponteiro] == '0' || entrada[ponteiro] == '.') && notDefaultRoute == false){
    			defaultRoute = true;
    			possivelIpDestino[ponteiro] = entrada[ponteiro];
    			ponteiro++;
    			stopLongestMatch == true;
    		}
    		else{
    			notDefaultRoute = true;
    			possivelIpDestino[ponteiro] = entrada[ponteiro];
    			ponteiro++;
    			stopLongestMatch == true;
    		}
    	}

    	if(numberMatch > longestMatch){
    		longestMatch = numberMatch;
    		indexEntradaLongestMatch = entradaAtual + 2;
    		//free(ipDestino);
    		strcpy(ipDestino, possivelIpDestino);
    		ponteiroFimIP = ponteiro;
    	}

    	if(defaultRoute == true && notDefaultRoute == false){
    		indexDefaultRoute = entradaAtual + 2;
    		ponteiroFimIP = ponteiro;
    	}

    	//printf("Iteração: %d\n", entradaAtual + 2);
    	defaultRoute = false;
   		notDefaultRoute = false;
    	numberMatch = 0;
    	stopLongestMatch = false;
    }

    //printf("entradaDoLongestMatch: %d\n", indexEntradaLongestMatch);
    //printf("longestMatch:%s\n", argv[indexEntradaLongestMatch]);
    //printf("Numero de entradas: %d\n", numeroDeEntradas);
    //printf("IP destino: %s\n", ipDestino);


    if(longestMatch > 0){

    	char *entrada = argv[indexEntradaLongestMatch];

		ponteiroFimIP++;

		int i = 0;
		while(entrada[ponteiroFimIP] != '/'){
			gateway[i] = entrada[ponteiroFimIP];
			ponteiroFimIP++;
			i++;
		}
		ponteiroFimIP++;
		i = 0;

		while(entrada[ponteiroFimIP] != '/'){
			mascara[i] = entrada[ponteiroFimIP];
			ponteiroFimIP++;
			i++;
		}
		ponteiroFimIP++;
		i = 0;

		while(entrada[ponteiroFimIP] != '\0'){
			interface[i] = entrada[ponteiroFimIP];
			ponteiroFimIP++;
			i++;
		}

		//printf("gateway destino: %s\n", gateway);

		//printf("longestMatch: %d\n", longestMatch);

    	printf("\nforwarding packet for %s to next hop %s over interface %s\n", ipOrigem, gateway, interface);
    }

    else if(indexDefaultRoute != -1){
    	char *entrada = argv[indexDefaultRoute];

		ponteiroFimIP++;

		int i = 0;
		while(entrada[ponteiroFimIP] != '/'){
			gateway[i] = entrada[ponteiroFimIP];
			ponteiroFimIP++;
			i++;
		}
		ponteiroFimIP++;
		i = 0;

		while(entrada[ponteiroFimIP] != '/'){
			mascara[i] = entrada[ponteiroFimIP];
			ponteiroFimIP++;
			i++;
		}
		ponteiroFimIP++;
		i = 0;

		while(entrada[ponteiroFimIP] != '\0'){
			interface[i] = entrada[ponteiroFimIP];
			ponteiroFimIP++;
			i++;
		}

		//printf("gateway destino: %s\n", gateway);

    	printf("\nforwarding packet for %s to next hop %s over interface %s\n", ipOrigem, gateway, interface);
    }

    else{
    	printf("\ndestination %s not found in routing table, dropping packet\n", ipOrigem);
    	//printf("gateway destino: %s\n", gateway);
    	//printf("indexDefaultRoute %d\n", indexDefaultRoute);
    }




    return 0;
}