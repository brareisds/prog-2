## Avaliação 3 - Space Invaders (Simulação de Tiros)

O projeto consiste na implementação da simulação dos tiros dos aliens no jogo Space Invaders. O objetivo do jogador é derrotar ondas de aliens com um laser móvel se movimentando horizontalmente.

## Funcionalidades Implementadas

- Implementação da simulação dos tiros dos aliens.
- Utilização de listas ligadas com sentinela para gerenciar os tiros.
- Implementação das funções necessárias para gerenciar os tiros dos aliens, como criar e remover tiros, atualizar a posição dos tiros e adicionar ou remover aliens do tabuleiro.

## Descrição de Execução

O usuário fornece as dimensões do tabuleiro, o número de linhas inimigas, a quantidade de iterações que o jogo irá evoluir/executar e um nome para o arquivo de saída (relatório).

### Forma de uso:
./space_enemies -y map_y_limit -x map_x_limit -e map_enemy_lines -r rounds -o output

## Funcionalidades a serem implementadas

### `A3.c`
- Função `execute_event` para atualizar a lista de tiros e verificar se novos tiros devem ser criados.

### `enemy.c`
- Função `remove_shot` para remover um tiro da lista.
- Função `update_shots` para atualizar a posição vertical dos tiros.
- Função `straight_shoot` para criar um novo tiro.
