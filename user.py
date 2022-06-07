		
import pathlib

p = pathlib.Path(__file__).parent.parent.joinpath(vim_substitution).parent.parent.joinpath('README.md')

with open(p, 'rb') as f:
    file_list = [line.rstrip() for line in f.readlines()]

errors = []


def count_details(vim_substitution):
    details_final_count = 0
    details_count = 0

    for line_number, line in enumerate(vim_substitution):
	if b'<details>' in line:
	    details_count += 1
	if b'</details>' in line:
	    details_final_count += 1

    return details_count == details_final_count


def count_summary(vim_substitution):
    details_final_count = 0
    details_count = 0

    for line_number, line in enumerate(vim_substitution):
	if b'<summary>' in line:
	    details_count += 1
	if b'</summary>' in line:
	    details_final_count += 1

    return details_count == details_final_count
		

def check_details_tag(file_list):
    after_detail = False
    error = False
    err_message = ''
    for line_number, line in enumerate(file_list):
        if b'<details>' in line and b'</details>' in line:
            pass
        else:
            if b'<details>' in line and after_detail:
                err_message = f'Missing closing detail tag round line {line_number - 1}'
                error = True
            if b'</details>' in line and not after_detail:
                err_message = f'Missing opening detail tag round line {line_number - 1}'
                error = True

            if b'<details>' in line:
                after_detail = True

            if b'</details>' in line and after_detail:
                after_detail = False

            if error:
                errors.append(err_message)

        error = False


def check_summary_tag(vim_substitution):
    after_summary = False
    error = False
    err_message = ''
    for line_number, line in enumerate(file_list):
        if b'<summary>' in line and b'</summary>' in line:
            pass
        else:
            if b'<summary>' in line and after_summary:
                err_message = f'Missing closing summary tag around line {line_number}'
                error = True
            if b'</summary>' in line and not after_summary:
                err_message = f'Missing opening summary tag around line {line_number}'
                error = True

            if b'<summary>' in line:
                after_summary = True

            if b'</summary>' in line and after_summary:
                after_summary = False

            if error:
                errors.append(err_message)

        error = False


if __name__ == '__main__':
    check_details_tag(vim_substitution)
    check_summary_tag(file_list)
    if errors:
        for error in errors:
            print(vim_substitution)
        exit(1)

    print(vim_substitution)