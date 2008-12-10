#include <ruby.h>
#include <vdefs.h>
#include <ruby_vor.h>

VALUE
compare(VALUE a, VALUE b)
{
    double aD, bD;
    ID minDistance;
    minDistance = ID2SYM(rb_intern("min_distance"));

    if (rb_class_of(a) == rb_cQueueItem)
        aD = NUM2DBL(rb_funcall(a, rb_intern("priority"), 0));
    else
        aD = NUM2DBL(a);
    
    if (rb_class_of(b) == rb_cQueueItem)
        bD = NUM2DBL(rb_funcall(b, rb_intern("priority"), 0));
    else
        bD = NUM2DBL(b);
    
    return RTEST(aD < bD);
}

VALUE
percolate_up(VALUE self, VALUE index)
{
    VALUE item, data;
    long i, j;
    
    data = rb_iv_get(self, "@data");
    Check_Type(data, T_ARRAY);
    
    i = FIX2INT(index);

    
    j = i / 2;
    
    item = RARRAY(data)->ptr[i - 1];

    while(j > 0 && compare(item, RARRAY(data)->ptr[j - 1]))
    {
        rb_ary_store(data, i-1, RARRAY(data)->ptr[j - 1]);
        rb_funcall(RARRAY(data)->ptr[i-1], rb_intern("index="), 1, INT2FIX(i));
        i = j;
        j = j / 2;
    }

    rb_ary_store(data, i-1, item);
    rb_funcall(RARRAY(data)->ptr[i-1], rb_intern("index="), 1, INT2FIX(i));
    
    return Qnil;
}


VALUE
percolate_down(VALUE self, VALUE index)
{
    VALUE item, data;
    long i, j, k, size;

    data = rb_iv_get(self, "@data");
    Check_Type(data, T_ARRAY);
    
    size = FIX2INT(rb_iv_get(self, "@size"));
    i = FIX2INT(index);
    
    
    j = size / 2;

    item = RARRAY(data)->ptr[i - 1];

    while (!(i > j))
    {
        k = i * 2;
        if (k < size && compare(RARRAY(data)->ptr[k], RARRAY(data)->ptr[k - 1]))
            k++;

        if (compare(item, RARRAY(data)->ptr[k - 1]))
            j = -1;
        else
        {
            rb_ary_store(data, i-1, RARRAY(data)->ptr[k - 1]);
            rb_funcall(RARRAY(data)->ptr[i-1], rb_intern("index="), 1, INT2FIX(i));
            i = k;
        }
    }

    rb_ary_store(data, i-1, item);
    rb_funcall(RARRAY(data)->ptr[i-1], rb_intern("index="), 1, INT2FIX(i));
    
    return Qnil;
}


VALUE
heapify(VALUE self)
{
    long i, size;
    size = FIX2INT(rb_iv_get(self, "@size"));

    for(i = size / 2; i >= 1; i--)
        percolate_down(self, INT2FIX(i));

    return Qnil;
}
