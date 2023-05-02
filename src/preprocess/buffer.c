static void buffer_destroy(void)
{
  for (uint64_t i = 0; i < definitions_buffer_size; i++)
  {
    __free(definitions_buffer[i]);
  }
  __free(definitions_buffer);

  for (uint64_t i = 0; i < returns_buffer_size; i++)
  {
    __free(returns_buffer[i]);
  }
  __free(returns_buffer);
}
