from pycspade.helpers import spade, print_result

result = spade(filename='file_python.txt', support=0.3, parse=False)
print(result['summary'])