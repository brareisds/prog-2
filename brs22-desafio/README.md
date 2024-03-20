
## Desafio - Implementação de novas funcionalidades para o Jogo Aggressive Squares
As modificações foram implementadas considerando o código da Fase IV do Projeto Quadrados Agressivos desenvolvido em sala de aula.

### Arquivos Modificados
As modificações foram feitas nos seguintes arquivos de código fonte:

- AggressiveSquares.c
- Square.c
- As linhas modificadas foram marcadas com o simbolo (!)

### Arquivos Adicionados
- As imagens RedHeart.bmp e BlueHeart.bmp foram adicionadas ao diretório do jogo para representar visualmente as vidas dos jogadores.

### Novas Funcionalidades Incorporadas:

# Representação Visual das Vidas
- Duas novas imagens de coração foram adicionadas ao jogo: "RedHeart.bmp" para o jogador 1 e "BlueHeart.bmp" para o jogador 2. Cada jogador começa com 3 vidas.
- As vidas dos jogadores são representadas na tela por corações vermelhos e azuis.
- Os jogadores perdem 1 (uma) vida quando são atingidos por projéteis inimigos.
- Quando um jogador perde todas as vidas, ele é derrotado.
- O tamanho das imagens de coração foi ajustado para se adequar ao tamanho da tela.

# Opção de Jogar Novamente
- Após o término do jogo (quando um jogador perde todas as vidas), os jogadores têm a opção de jogar novamente pressionando a tecla ESPAÇO.
- Se a tecla ESPAÇO for pressionada, os jogadores começam um novo jogo com todas as vidas e atributos restaurados.
- Se os jogadores optarem por não jogar novamente, o jogo pode ser encerrado pressionando a tecla ESC.


# Estruturas Modificadas
- Foi adicionado o membro `unsigned short lifeCount` na struct `square` para armazenar a quantidade de vidas de cada jogador.

# Funções Modificadas
- `check_kill`: Foi adicionada a remoção do projetil da fila de tiros quando ele atinge um jogador e o decremento do LifeCount da vitima. A função retorna 1 quando o LifeCount chega em 0 ou retorna 0 caso contrário.
- `update_position`: Foi adicionada a variável `NewUpperYScreen` como um novo parâmetro para ser usado pela função `square_move`.
- `square_move`: Usa a variável `NewUpperYScreen` acrescentada como parâmetro para ajustar o limite superior da tela no momento de calcular os movimentos dos jogadores garantindo que os jogadores não sobreponham a representação visual das vidas.

# Funções adicionadas:
- A função `checkPlayerLives` foi adicionada para desenhar os corações na tela com base na quantidade de vidas dos jogadores. Ela é chamada a cada atualização da tela e usa as imagens dos corações para calcular a posição dos mesmos.

# Modificações na main
- Foi adicionada a biblioteca `allegro_image-5` para representar visualmente as vidas dos jogadores, junto com a função `al_init_image_addon()` reponsável por inicializar o módulo de manipulação das imagens.
- As imagens de coração vermelho e azul foram incluídas no jogo, a partir da declaração e inicialização das novas variáveis `ALLEGRO_BITMAP *RedHeart e ALLEGRO_BITMAP *BlueHeart`
- As funções `al_get_bitmap_width()` e `al_get_bitmap_width()` foram adicionadas para ajustar a altura e largura das imagens antes de exibi-las na tela
- A atribuição da variável `NewUpperYScreen` foi feita com base na maior proporção de altura da imagem.
- A mensagem de encerramento do programa foi atualizada para indicar que os jogadores podem pressionar a tecla ESPAÇO para jogar novamente ou a tecla ESC para sair.
- Caso a tecla ESPAÇO seja pressionada, os jogadores e seus atributos são reinicializados.
- Uma linha é desenhada no inicio da tela para indicar o novo limite superior dos jogadores definido pela variavel ` NewUpperYScreen`
