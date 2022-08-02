from __future__ import print_function

import ttfw_idf

TASK_ITERATION_LIMIT = 10

TASK_ITERATION_POSTING = 'posting TASK_EVENTS:TASK_ITERATION_EVENT to {}, iteration {} out of ' + str(TASK_ITERATION_LIMIT)
TASK_ITERATION_HANDLING = 'handling TASK_EVENTS:TASK_ITERATION_EVENT from {}, iteration {}'


@ttfw_idf.idf_example_test(env_tag='Example_GENERIC', target=['esp32', 'esp32c3'])
def test_user_event_loops_example(env, extra_data):
    dut = env.get_dut('user_event_loops', 'examples/system/esp_event/user_event_loops')

    dut.start_app()

    dut.expect('setting up')
    dut.expect('starting event source')
    dut.expect('starting application task')
    print('Finished setup')

    for iteration in range(1, TASK_ITERATION_LIMIT + 1):
        loop = None

        loop = 'loop_with_task' if (iteration % 2 == 0) else 'loop_without_task'
        dut.expect(TASK_ITERATION_POSTING.format(loop, iteration))
        print(f'Posted iteration {iteration} to {loop}')
        dut.expect(TASK_ITERATION_HANDLING.format(loop, iteration))
        print(f'Handled iteration {iteration} from {loop}')

    dut.expect('deleting task event source')
    print('Deleted task event source')


if __name__ == '__main__':
    test_user_event_loops_example()
