class Name(str):
  pass

def parse(text):
  commands = []

  i = 0

  while True:
    while text[i:i+1].isspace():
      i += 1

    if i >= len(text):
      break

    j = i
    if text.startswith(('r"""', "r'''", "'", '"'), i):
      if text[i:i+1] == 'r':
        raw = True
        i += 1
      else:
        raw = False

      if text.startswith(('"""', "'''"), i):
        quote = text[i:i+3]
        i += 3
      else:
        quote = text[i]
        i += 1

      while not text.startswith(quote, i):
        if i >= len(text):
          raise SyntaxError()

        if not raw and text[i] == '\\':
          i += 2
        else:
          i += 1

      i += len(quote)
      commands.append(eval(text[j:i]))

    else:
      j = i
      while i < len(text) and not text[i].isspace():
        i += 1

      value = text[j:i]

      try:
        commands.append(float(text[j:i]))
      except ValueError:
        commands.append(Name(text[j:i]))

  return Program(commands)

class Program(object):

  def __init__(self, commands):
    self.commands = commands
    self.ip = 0

  def step(self):
    pass

  def __repr__(self):
    return 'Program(%r)' % self.commands

print parse('a b c 54 "hey there"')
