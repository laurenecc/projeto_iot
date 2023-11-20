# projeto_iot
Sistema IoT para Medir Temperatura e Umidade Local

Resumo. Utilizando o princípio de Internet das Coisas para medir a temperatura e umidade local. O hardware utilizado nesta implementação é composto por um microcontrolador ESP32, que controla um sensor BME280 para coletar os dados da temperatura e da umidade local. Caso o valor da temperatura e umidade local, atinjam uma marca pré-determinada, um atuador acionará de forma automática um umidificador posicionado em determinado ambiente. Da mesma forma, quando a umidade atingir valores satisfatórios, o atuador deverá desligar o umidificador de ar automaticamente.

1. A Internet das Coisas e a Sustentabilidade
A Internet das Coisas (IoT – Internet of Things) é uma tecnologia que possibilita a conexão entre diversos dispositivos e objetos, seja em ambiente domiciliar, corporativo ou áreas urbanas.
	A capacidade de conectar objetos e dispositivos é viabilizada graças aos avanços na área da Inteligência Artificial (IA) e Aprendizado de Máquina (Machine Learning). Com isso, as máquinas podem processar dados e aprender, tornando-se cada vez mais inteligentes e eficientes em suas funções. Essa automatização e a capacidade de otimizar o uso de recursos, destacam a importância da IoT, não somente na área da saúde, educação e transporte, mas também na área da sustentabilidade.
	Um dos exemplos de forte impacto ambiental com a utilização da IoT, é no setor agrícola. Neste contexto, a IoT pode apresentar dispositivos capazes de monitorar o estado do solo, incluindo níveis de umidade e nutrientes, além de apoiar a otimizar o uso adequado de recursos hídricos e fertilizantes.
	Outro exemplo interessante que podemos citar, são dispositivos menos intrusivos, que utilizam a geolocalização para rastrear animais ameaçados de extinção, monitorando seu estado de saúde e fornecendo informações mais precisas para os cientistas, contribuindo assim, para a conservação dessas espécies.

2. Materiais e Métodos
2.1 Materiais
2.1.1 Microcontrolador ESP32
Para este projeto, vamos utilizar o microcontrolador da marca ESPRESSIF de modelo ES-P32 que possui antena embutida, conector micro-usb, módulo Wi-Fi, Wroom-32 e Bluetooth BLE-0287.
Tensão de operação de 3.3V com capacidade de voltagem mínima de entrada limitada a 5V e voltagem máxima de entrada limitada a 9V. CPU Xtensa Dual-Core 32-bit LX6.
ROM de 448 Kbytes e RAM de 530 Kbytes. Flash de 4 MB. Clock máximo de 240MHz. Wireless padrão de 802.11 b/g/n.
Conexão Wi-Fi de 2.4Ghz com um máximo de 150 Mbps. Wi-Fi Direct (P2P), P2P Discovery, P2P Group Owner mode e P2P Power Management.
Modos de operação STA/AP/STA+AP; Bluetooth BLE 4.2; Portas GPIO 11; GPIO com funções de PWM, I2C, SPI. Taxa de transferência de 110-460800bps.
Suporta Upgrade remoto de firmware e possui conversor analógico digital (ADC).
A distância entre os pinos é de 2,54 mm e as dimensões totais do componente são de aproximadamente 52 mm x 28 mm x 5 mm.

2.1.2 Sensor de Umidade e Temperatura BME280
Como sensor de umidade e temperatura, vamos utilizar o modelo BME280 da marca Bosch Sensortec. Este modelo permite fazer leituras de temperaturas entre -40 a +85 graus Celsius; leitura de pressão entre 300 e 1100 hPa e umidade entre 0 e 100% UR. O sensor possui tensão de operação de 1,8 até 3,6VDD; interface de comunicação I2C com até 3,4MHz e SPI com até 10MHz.
A margem de erro das leituras de umidade, temperatura e pressão é na faixa de ±0,008%RH; ±0,01ºC e ±1Pa respectivamente.
A precisão da medição de umidade é por volta de ±3% UR.
O sensor também possui histerese ≤ 2%. Seu tempo de resposta é de apenas 1 segundo e sua dimensão é de aproximadamente 2,5 x 2,5 x 0,93 mm³.

2.1.3 Relay como Atuador Eletromagnético
Vamos utilizar um relay como um atuador eletromagnético da marca Songle, com 1 canal de 5V que funciona com Arduino, ESP32, ESP8266, Raspberry Pi ou qualquer microcontrolador de 5V ou 3,3V.

O relay possui as seguintes especificações:
Normalmente aberto e normalmente fechado.
Módulos definidos em níveis altos ou baixos através de jumpers.
Isolamento do optoacoplador patch.
Blocos terminais para entrada e saída (input and output).
Dois indicadores LED de potência verde e o status do relay em vermelho.
Tensão operacional de 5V.
Tensão de disparo de 3,3V a 5V.
Corrente de disparo de 5mA.
Carga máxima de AC 250V/10A, DC 30V/10A.
Dimensão de 50 mm x 26 mm x 18,5 mm.

2.2 Métodos
O projeto é composto por sua parte em hardware, já detalhada acima, e uma parte em software. Sua parte em hardware é responsável por monitorar o ambiente local, ou seja, o sensor BME280 faz a mensuração local de temperatura e umidade, esses dados coletados serão validados e então encaminhados para uma base online através do protocolo MQTT. A parte em software é responsável por manter uma base e realizar a comparação da mensuração com o valor desejado, para determinar a ação do atuador.
		A cada mensuração de temperatura e umidade, os dados são comparados com um valor de ação pré-determinado. Ou seja, caso a temperatura esteja acima do valor determinado e/ou a umidade esteja abaixo do valor determinado, o atuador será acionado para ligar um umidificador de ar. Após novas mensurações, caso seja identificado que a temperatura e/ou a umidade voltou para o valor desejado, o atuador será acionado novamente para desligar o umidificador de ar.

3 Interface do Sensor BME280 com a Placa ESP32
Abaixo temos a tabela que descreve como conectar os pinos do sensor BME280 com a placa ESP32:

Tabela 1. Interface BME280 com ESP32

BME280	ESP32
Vin	Conectado com 3.3V
GND	GND
SCL	GPIO22
SDA	GPIO21

4 Atuador Eletromagnético
Entendemos que podemos conectar um LED diretamente com a placa ESP32, mas não podemos conectar dispositivos de alta tensão, como um umidificador de ar, que é a proposta do projeto. Para não queimar a placa ESP32 vamos utilizar um relay como atuador.
		Relay ou relé, é um tipo de atuador eletromagnético que pode ser utilizado desde interruptores simples até sistemas de automação complexos. Sua função principal consiste em converter energia elétrica em movimento mecânico, por meio da aplicação de forças eletromagnéticas, ou seja, quando uma corrente elétrica percorre a bobina de um relay, ela gera um campo magnético. Esse campo magnético exerce atração sobre um contato metálico, resultando na abertura ou fechamento de um circuito elétrico.

5. Interface do Relay com a Placa ESP32
Abaixo temos a tabela que descreve como conectar os pinos do relay com a placa ESP32:

Tabela 2. Interface do Relay com a placa ESP32

Relay	ESP32
DC+	Conectado com o vin 5V
IN	GPIO13
DC-	GND

6 Protocolo MQTT
MQTT (Message Queue Telemetry Transport) é um protocolo de comunicação baseado em padrões, um conjunto de regras que viabiliza a troca de mensagens entre computadores. O MQTT é amplamente utilizado na Internet das Coisas (IoT), onde sensores inteligentes e outros dispositivos precisam se comunicar em redes com recursos limitados e largura de banda restrita.
		O protocolo MQTT opera com um modelo de Publish e Subscribe, que permite que os clientes enviem e recebam informações enquanto um servidor, chamado Broker, gerencia essa troca de dados. Isso quer dizer que em uma configuração MQTT, existe um Publisher responsável por enviar mensagens para um tópico específico, e um Subscriber que se inscreve nesse tópico para receber as mensagens.

7. Montagem de um cabo com o Relay
Para concluir o propósito do projeto, que seria fazer o atuador ligar um umidificador de ar automaticamente, conectamos um cabo pp de 1,5m com a interface de saída do relay, conforme descrito na tabela 3 que segue logo mais abaixo.
		O cabo pp possui em uma ponta um plug fêmea e na outra ponta um plug macho. O plug macho será conectado com uma fonte de energia na parede, para fornecer a energia que irá ligar o umidificador de ar (ventilador). Já o plug fêmea, será utilizado para conectar o plug macho do umidificador de ar (ventilador). A função do relay nesse ponto, será de “abrir” a saída de energia no cabo, quando o sensor atingir uma marca de temperatura alta e umidade baixa e cortar a “saída” de energia do cabo, quando a temperatura e umidade atingir um valor desejado.

Tabela 3. Interface do Relay com um Cabo pp
Relay	Cabo pp
NC	Não conectado
COM	Saída do cabo ao plug macho
NO	Saída do cabo ao plug fêmea

8. Desenvolvimento
8.1 Configurando a placa ESP32 no Arduino IDE
Para capturar a leitura de umidade e temperatura do sensor BME280, vamos conectar o cabo USB do microcontrolador ESP32 a um computador com Sistema Operacional Windows 10 de 64 bits. Assim que o computador reconhecer o driver da placa, vamos utilizar o Arduino IDE 2.2.1 para realizar a parte de programação do código do projeto. Para tanto, será necessário configurar o software para que possamos trabalhar com a placa ESP32. Já com o Arduino aberto, vamos instalar os pacotes da placa ESP32 clicando em Arquivo > Preferências (File > Preferences).

Na tela que se apresenta, devemos inserir os dois caminhos URL na parte “URLs adicionais do gerenciador de quadros” (Additional boards manager URLs): 
http://arduino.esp8266.com/stable/package_esp8266com_index.json, 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Após isso, vamos instalar o pacote do ESP32 da Espressif Systems. No menu principal do IDE vamos escolher o caminho Ferramentas > Quadro > Gerenciador de Quadros... (Tools > Board > Boards Manager…). Na barra de pesquisas que é apresentada na lateral esquerda da interface, digitamos ESP32 e escolhemos Espressif para instalar.

8.2 Configurando o sensor BME280 no Arduino IDE
Para utilizar o sensor BME280, precisamos instalar duas bibliotecas da Adafruit no Arduino IDE. No seguinte caminho Sketch > Incluir Biblioteca > Gerenciar Bibliotecas (Sketch > Include Library > Manage Libraries) vamos pesquisar por “Adafruit bme280” na caixa de pesquisas que é apresentada e escolher instalar a biblioteca. Após isso, vamos pesquisar por “Adafruit unified sensor” e escolher instalar.

8.3 Definindo a placa ESP32 no Arduino IDE
Para conseguirmos programar a placa ESP32 no Arduino IDE, vamos selecionar o “quadro” (board) necessário no caminho Ferramentas > Quadro > esp32 > ESP32 Dev Module (Tools > Board > esp32 > ESP32 Dev module). 
Além disso, precisamos apontar em qual porta a placa ESP32 está conectada. No nosso caso o nome da porta é COM8, conforme apresentado na imagem abaixo. Para selecionar essa porta, vamos seguir o caminho Ferramentas > Porta > COM8. (Tools > Port > CM8).

9. Configurando o protocolo MQTT
Para finalizar nosso projeto, precisamos de uma conexão à Internet, para esse propósito, vamos utilizar o protocolo MQTT. Vamos instalar o MQTT Explorer em nossa máquina e realizar as configurações necessárias para a transmissão das mensagens de leitura da temperatura e umidade do nosso sensor BME280.
Após instalar o MQTT Explorer, vamos cadastrar os tópicos necessários para o envio das leituras. Para isso, na janela inicial que é apresentada ao executar o programa, escolhemos “test.mosquitto.org”. Nesta parte não é necessário modificar nada, então clicamos em “Advanced” para cadastrar os seguintes tópicos: topicoBME/Umidade; topicoBME/Temperatura. Após digitar o primeiro tópico, basta clicar em +ADD.
