# ❤️ Health Care

Health Care é um projeto que tem como objetivo o monitoramento remoto e automático de pacientes para que 

## Descrição
Este projeto consiste em um sistema de monitoramento de pacientes hospitalares, desenvolvido para acompanhar e registrar informações vitais dos pacientes em tempo real, fornecendo uma interface amigável para médicos e equipe médica.

## Membros
- Allan Von Ivanov
- Cássio Yonetsuka

## Funcionalidades

### 1. Registro e Visualização de Dados
- **Monitoramento em Tempo Real:** Monitora constantemente a temperatura ambiente, umidade.
- **Registro de Dados:** Armazena os dados coletados em um banco de dados.

### 2. Alertas e Notificações
- **Alertas de Condições:** Dispara alertas imediatos caso algum dado esteja fora dos parâmetros normais.
- **Notificações para Equipe Médica:** Envia notificações para os dispositivos da equipe médica responsável pelo paciente.

## Objetivos
- **Aprimorar o Monitoramento Médico**
- **Facilitar análise de dados**
- **Garantir segurança**

## Dores a suprimir
- **Limitações no monitoramento**
- **Atrasos na detecção de problemas**

## Registro no postman
Para realizar este projeto, foi usado do fiware descomplicado do Professor Cabrini,<a href="https://github.com/fabiocabrini/fiware">Clique aqui para ver</a>.
Ao baixar, voce abre o postman e exporta os comandos. Depois substitui url pelo endereço do broker, vai em "3. Provisioning a Smart Lamp", vá em body e substitua lamp e lamp001 pelo nome que voce quiser registrar e insira os tópicos que voce quiser informando o nome e tipo(int, string, float, etc).
Por fim, verifique se o seu dispositivo foi registrado em "5. List all Devices Provisioned".

## Tecnologias utilizadas
- WiFi:
**Biblioteca utilizada para facilitar a conexão e configuração da rede WiFi**
- PubSubClient:
**Biblioteca utilizada para implementação MQTT**
- LiquidCrystal_I2C:
**Biblioteca utilizada para usar o lcd_I2c**
- DHTesp:
**Biblioteca utilizada para usar o sensor DHT no Esp**

## Link para projeto em wokwi:

https://wokwi.com/projects/381547877876523009
