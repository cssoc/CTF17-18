#use 5 bit LSFR, x5, x3, 1


class LSFR_5bit:
    poly = 20

    def __init__(self, state):
        self.state = state % 2**5

    def get_bit(self):
        output = self.state & 1
        self.state >>= 1;
        if(output % 2 == 1):
            self.state ^= type(self).poly
        return output

    def get_byte(self):
        return self.get_n_bits(8)
        

    def get_n_bits(self, n):
        output = self.get_bit()

        for x in range(n-1):
            output <<= 1
            output |= self.get_bit()

        return output
