class LINQ:
    def __init__(self, some_gen):
        self.gen = some_gen

    def Where(self, condition):
        tmp_gen = self.gen()
        def generate():
            for element in tmp_gen:
                if condition(element):
                    yield element
        self.gen = generate
        return self

    def Select(self, condition):
        tmp_gen = self.gen()
        def generate():
            for element in tmp_gen:
                yield condition(element)
        self.gen = generate
        return self

    def Take(self, num_first_elem):
        tmp_gen = self.gen()
        def generate():
            for _ in range(num_first_elem):
                yield next(tmp_gen)
        self.gen = generate
        return self

    def ToList(self, query_list):
        for element in self.gen():
            query_list.append(element)
        return query_list

def InfiniteFibonacci():
    a, b = 1, 0
    while True:
        a, b = b, a + b
        yield b

query = list()
seq = LINQ(InfiniteFibonacci)
seq.Where(lambda x: not x % 3).Select(lambda x: x ** 2 if not x % 2 else x).Take(5).ToList(query)
print(query)

# [3, 21, 20736, 987, 6765]