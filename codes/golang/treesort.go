// treesort 排序
package tree

type tree struct {
	value int
	left,right *tree
}

// 就地排序
func Sort(values []int){
	var root *trees
	for _,v: range values{
		root =add(root,v)
	}
	appendValues(values[:0],root)
}

// appendValues 将元素按顺序追加到values里面，然后返回结果slice
func appendValues(values []int,t*tree){
	if t!=nil {
		values=appendValues(values,t.left)
		values=append(values,t.value)
		values=appendValues(values,t.right)
	}
	return values
}

func add(t *tree,value int){
	if t==nil {
		// 等价于返回&tree{value,value}
		t=new(tree)
		t.value=value
return t
	}
	if value<t.value {
		t.left = add(t.left,value)
	}else {
		t.right = add(t.right,value)
	}
}