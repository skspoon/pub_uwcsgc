"""
Task to solve:
Write a function that will transform an array of integers and/or
arbitrarily nested arrays of integers into a flat array of integers.
e.g.: [[1,2,[3]],4] -> [1,2,3,4].

Assumptions:
Doing sorting or detecting duplicate were not a part of the task.
"""


ot_list = list()

def unwind(in_list):
    """
    Recursively scans, and decend into nested list, and returns
    when a an integer is extracted and add to the "flat" list.
    """

    if isinstance(in_list, int):
        ot_list.append(in_list)
    elif in_list:
        unwind(in_list.pop(0))
        unwind(in_list)


def test_unwind():

    # ll = [[1, 2, [3]], 4]
    ll = [[1, 2, 3, [3, 4]], 4, 5]
    print("Input list: {}".format(ll))

    unwind(ll)
    print("Output list: {}".format(ot_list))


if __name__ == "__main__":
    test_unwind()
