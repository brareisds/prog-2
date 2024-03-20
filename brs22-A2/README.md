## Avaliação 2 (continuação da av1) - Leitor de Atributos ARFF

O projeto consiste na implementação de um leitor de atributos de arquivos ARFF (Attribute-Relation File Format). O formato ARFF é utilizado no contexto da solução WEKA (Waikato Environment for Knowledge Analysis) para armazenar dados em formato de tabela.

## Funcionalidades Implementadas

- Abrir um arquivo no formato ARFF.
- Ler a seção de atributos do arquivo ARFF.
- Armazenar o rótulo, tipo e categorias de cada atributo em uma estrutura de dados.
- Retornar um vetor de struct atributo contendo cada atributo do arquivo ARFF, com ordenamento coincidente com a ordem de aparição dos atributos no arquivo.
- Tratar erros na definição da seção de atributos do arquivo ARFF, retornando NULL em caso de problema.

## Detalhes de Implementação

- Uma linha de atributo não iniciada com "@attribute" é tratada como erro.
- Linhas em branco antes dos atributos, entre atributos e entre um atributo e o token "@data" são simplesmente ignoradas.
- Nenhuma linha de atributo tem mais do que 1024 caracteres.
- Há exatamente um espaço entre um elemento e outro em uma linha de atributo.
- Não há espaços no início e no final de uma linha de atributos.
- Atributos categóricos têm seus valores sempre definidos corretamente.

## Observações 

- Uma linha de atributo pode conter menos ou mais elementos, além dos necessários. O programa trata esse possível erro.
- Em caso de qualquer problema na definição da seção de atributos (ou ausência do token "@data") do arquivo ARFF fornecido como entrada, a função retorna NULL.

