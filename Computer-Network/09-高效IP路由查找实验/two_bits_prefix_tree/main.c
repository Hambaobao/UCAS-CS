#include "two_bits_prefix_tree.h" 
#include "tool.h"


int main()
{
	construct_tree();

	// 手动查询时使用
	// look_up_one();

	// 查询所有IP
	look_up_all();

	// 检查所有结果
	check_results();

	return 0; 
}