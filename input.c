static inline int* win32_vkey_to_key(UINT key)
{
	switch (key)
	{
		case VK_RETURN: return &input.enter;
		case VK_DOWN: return &input.down;
		case VK_RIGHT: return &input.right;
		case VK_LEFT: return &input.left;
		case 'A': return &input.a;
		case 'D': return &input.d;

		default: return NULL;
	}
}

static inline void clear_input()
{
	memset(&input, 0, sizeof(input_t));
}
