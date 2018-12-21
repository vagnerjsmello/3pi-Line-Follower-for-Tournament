# RobinBot: 2º Lugar no RJ na categoria Seguidor de linha 
#### Motivação:
Pesquisas e muita dedicação para melhorar o desempenho do 3PI Pololu para participação do 1º Torneio de Robótica do CEFET na categoria Seguidor de Linha PRO.

#### Conteúdo:
Esse repositório contém:
- Código fonte do seguidor de linha Pololu 3PI usado no 1º Torneio de Robótica do RJ, organizado pelo CEFET.
- Guia de instalação das bibliotecas Orangutan e Pololu 3PI.
- Guia de instalação e uso do programador AVR USBasp no Pololu 3PI.

## Código Fonte
#### Motivação:
Compartilhar e ajudar novos competidores no entendimento das funcionalidades e lógica do seguidor de linha. 

#### Alterações e adição de funcionalidades:
O seguidor de linha, **Robin Bot**, baseia-se nas bibliotecas Orangutan e Pololu 3PI e exemplos, oferecido pela Pololu. Fiz algumas modificações, adicionei métodos e novas funcionalidades:
- Melhoria no PID;
- Configuração de Velocidade;
- Configuração de tempo de parada;
- Tempo de corrida (contador de segundos);

## USBasp
#### Motivação:
Tínhamos o seguidor de linha 3PI Pololu, mas não tínhamos o programador AVR oficial. Então usei o USBasp 10 pinos, sendo necessário fazer o mapeamento 10 para 6, uma vez que o 3pi Pololu possuí apenas 6 pinos. Informações sobre mapeamento dos pinos e instalação do drive e uso da IDE Arduino estão no arquivo: **tutorial_ArduinoIDE_e_USBASP.docx**


## Resultado Final: 2º Lugar no Torneio
#### Vídeos 
Você pode conferir a playlist da evolução do Robin Bot (3PI Pololu) aqui:  [https://www.youtube.com/playlist?list=PLdLf_16VKIelj-zjcngmMQi77mYgT0no6](https://www.youtube.com/playlist?list=PLdLf_16VKIelj-zjcngmMQi77mYgT0no6)

#### Equipe Infoboto 
![Equipe Infoboto](https://raw.githubusercontent.com/vagnerjsmello/3pi-Line-Follower-for-Tournament/master/images/equipe_infoboto.jpg)

#### Ajustes e programação
![Ajustes e programação](https://raw.githubusercontent.com/vagnerjsmello/3pi-Line-Follower-for-Tournament/master/images/robinbot_ajustes.jpg)

#### 1º Troféu da equipe Infoboto
![Robinbot - Segundo lugar](https://raw.githubusercontent.com/vagnerjsmello/3pi-Line-Follower-for-Tournament/master/images/robinbot_segundo_lugar.jpg)
