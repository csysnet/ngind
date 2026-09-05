void ngd_file_init(ngd_file_t *file);

int ngd_file_open(ngd_file_t *file, const char *path);

int ngd_file_create_temp(ngd_file_t *file);

int ngd_file_read(ngd_file_t *file,
    void *buf, size_t size,
    size_t *bytes_read);

int ngd_file_write_all(ngd_file_t *file,
    const void *buf, size_t size);

int ngd_file_close(ngd_file_t *file);
