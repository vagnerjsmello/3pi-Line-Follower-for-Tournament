# Seguidor de linha
Esse repositório contém:
- Código fonte do seguidor de linha Pololu 3PI usado no 1º Torneio de Robótica do RJ, organizado pelo CEFET.
- Guia de instalação das bibliotecas Orangutan e Pololu 3PI.
- Guia de instalação e uso do programador AVR USBasp no Pololu 3PI.

## Código Fonte
#### Motivação:
Compartilhar e ajudar novos competidores no entendimento das funcionalidades e lógica do seguidor de linha. Esse código foi criado para participação do 1º Torneio de Robótica do CEFET na categoria Seguidor de Linha.

#### Alterações e adição de funcionalidades:
Nosso seguidor de linha, o **Robin Bot**, baseia-se nas bibliotecas Orangutan e Pololu 3PI e exemplos, oferecido pela Pololu. Fiz algumas modificações, adicionei métodos e novas funcionalidades como:
- Melhoria no PID;
- Configuração de Velocidade;
- Configuração de tempo de parada;
- Tempo de corrida (contador de segundos);

## USBasp
#### Motivação:
Tínhamos o seguidor de linha 3PI Pololu, mas não tínhamos o programador AVR oficial. Então usamos o USBasp 10 pinos e foi necessário fazer o mapeamento dos pinos, uma vez que o 3pi Pololu possuí apenas 6 pinos. Informações sobre mapeamento dos pinos e instalação do drive e uso da IDE Arduino estão no arquivo: tutorial_ArduinoIDE_e_USBASP.docx

## Vídeos 
Você pode conferir a playlist da evolução do Robin Bot (3PI Pololu) aqui:  [https://www.youtube.com/playlist?list=PLdLf_16VKIelj-zjcngmMQi77mYgT0no6](https://www.youtube.com/playlist?list=PLdLf_16VKIelj-zjcngmMQi77mYgT0no6)

