class LINQ:
    def __init__(self, some_gen):
        self.gen = some_gen

    def Flatten(self):
        tmp_gen = self.gen()
        flat_list = list()
        for arr in tmp_gen:
            for item in arr:
                flat_list.append(item)
        def generate():
            for item in flat_list:
                yield item
        self.gen = generate
        return self

    def GroupBy(self, condition):
        tmp_gen = self.gen()
        vocab = dict()
        for item in tmp_gen:
            new_item = condition(item)
            if vocab.get(new_item) is not None:
                vocab[new_item].append(item)
            else:
                vocab[new_item] = [item]
        def generate():
            for item in vocab:
                yield (item, vocab[item])
        self.gen = generate
        return self

    def Select(self, condition):
        tmp_gen = self.gen()
        def generate():
            for item in tmp_gen:
                new_item = condition(item)
                yield new_item
        self.gen = generate
        return self

    def OrderBy(self, condition):
        my_seq = self.gen()
        sorted_list = list()
        for item in my_seq:
            sorted_list.append(item)
        sorted_seq = sorted(sorted_list, key=condition)
        def generate():
            for item in sorted_seq:
                yield item
        self.gen = generate
        return self

    def ToList(self, query_list):
        for item in self.gen():
            query_list.append(item)
        return query_list

def FileReader(file_name):
    text = open(file_name)
    def generate():
        for string in text:
            yield string
    return generate

query = list()
seq = LINQ(FileReader("input.txt"))
seq.Select(lambda x: x.split()).Flatten().GroupBy(lambda x: x).Select(lambda x: (x[0], len(x[1]))).OrderBy(lambda x: x[1]).ToList(query)
print(query)
